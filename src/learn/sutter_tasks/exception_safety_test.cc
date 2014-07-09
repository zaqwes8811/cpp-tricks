/// New tasks - p. 81
// безопасность искл. - часть проектирования
//
// Troubles:
//   - Resources
//   - Consistency - Invariants
//
// Guards, transactions
//
// !!!Error handling strategy - это самое важное!!! Это не деталь реализации
//   Сообщения об ошибках, Распространение ошибок, Обработка
// Важно то что обработчик должен иметь достаточно информации - например пользователь
//
// "...обрабатывает сам и остается в согласованном состоянии..."
// Интересно сделано. Классы исключения включены в интерфейс класса, как внутр. классы
//
// Try/Catch - обработка ошибок, преобразование искл., перехват на границе системы
//
// Warranty
// - Basic - нет утечек и объекты в согл., но возможно непредсказуемом состоянии
// - Strong - обеспечивает семантику транзакций
// - Nofail - бессбойность - destructors, resource free, swap
//TODO: Когда какую? Минимально строгую, в которой не нуждается вызывающий код. - don't understood...
//
/// Own Summary:
// - RAII, ScopeLocks
// - one function - strongly one task
// - calc and change state object use exc. safe methods - транзакционное программирование
// - base types and pointer (smart?) on assign no throw - TODO: = can throw, how impl swap? pimpl?
//   конструктор, констр. копирования и опер. присв. могут бросать исключения.


#include <stdexcept>
#include <algorithm>
#include <iostream>

#include <gtest/gtest.h>

using std::logic_error;
using std::cout;

class Session {
public:
  // Inner class:
  // http://bytes.com/topic/c/answers/133914-static-nested-class
  // http://stackoverflow.com/questions/3846696/nested-classes-c-static-inner-methods-xml-parsing-and-trying-to-populate-vect
  //
  // Inh.:
  // http://stackoverflow.com/questions/15777635/c-calling-base-class-constructors
  /*static*/ class OpenFailed : public logic_error {
  public:
    OpenFailed() : logic_error("") {

    }
  };

  // Throws: OpenFailed
  void Open() {
  }
};


TEST(Sutter, ExceptionBase) {
  Session::OpenFailed error();

}

// safe stack
// Требования к Т довольно строгие.
//   - ..
//   - Безопасное присваивание - ?
//   - конструктор по умолчанию
//TODO: снизить требование
template<class T> class Stack
{
public:
  Stack();
  ~Stack();

  // Step 3
  Stack(const Stack&);
  Stack& operator=(const Stack&);
  T* NewCopy(const T* src, size_t srcsize, size_t destsize);

  // Step 4
  size_t Count() const;
  void Push(const T&);
  // решают две задачи
  T Pop_nosafe();  // if empty throw
  void Pop_better_but(T& result);
  // safe
  T& Top();  //TODO: const?
  void Pop();

private:
  T* v_;
  size_t vsize_;
  size_t vused_;  // количество реально исп. объектов
};

//TODO: я не верю что это безопасно! это неуправлемый ресурс же?
template<class T>
Stack<T>::Stack() :
  //http://stackoverflow.com/questions/377178/how-does-the-standard-new-operator-work-in-c
  // "new handles constructor exceptions"
    // TODO: как вызовется delete []?!
    // http://ptgmedia.pearsoncmg.com/imprint_downloads/informit/aw/meyerscddemo/DEMO/MAGAZINE/SU_FRAME.HTM
    v_(/*0 step 2*/new T[10]),  // может сгенерировать и new and T()
    vsize_(10),
    vused_(0) {
  //v_ = new T[vsize_];  // Step 2: лучше передать в списке иниц. Мишко похожее говорил в GTechTalk
  // T::T() may throw
}
template<class T>
Stack<T>::~Stack() {
  delete [] v_;
  // T::~T() // throw()  // must be!!
}

// Step 3
template<class T>
T* Stack<T>::NewCopy(const T* src, size_t srcsize, size_t destsize) {
  using std::copy;
  assert(destsize >= srcsize);
  T* dest = new T[destsize];
  try {
    copy(src, src+srcsize, dest);  //DANGER: used =,  не ясно в чем проблема, но она есть
  } catch (...) {
    delete [] dest;
    throw;
  }
  return dest;
}

// Step 3: neutral
template<class T>
Stack<T>::Stack(const Stack<T>& other) :
  v_(NewCopy(other.v_,
             other.vsize_,
             other.vsize_)),  // strange
  vsize_(other.vsize_),
  vused_(other.vused_) {
}

// Step 3: assign
template<class T>
Stack<T>& Stack<T>::operator=(const Stack<T>& other) {
  if (this != &other) {
      // may throw
      T* v_new = NewCopy(other.v_, other.vsize_, other.vsize_);

      // must not throw - change state object
      delete [] v_;
      v_ = v_new;
      vsize_ = other.vsize_;
      vused_ = other.vused_;
  }
  return *this;
}

// Step 4
template<class T>
size_t Stack<T>::Count() const  { return vused_; }

template<class T>
void Stack<T>::Push(const T& t) {
  if (vused_ == vsize_) {
    // нужно увеличить память
    // may throw - isolated code
    size_t vsize_new = vsize_*2+1;
    T* v_new = NewCopy(v_, vsize_, vsize_new);

    // no throw
    delete[] v_;
    v_ = v_new;
    vsize_ = vsize_new;
  }
  v_[vused_] = t;  //DANGER: if throw - then no Strong Warr.
    // Итераторы станут не действ.
  ++vused_;  // pointer to next
}

// Step 5
// bad pop
// Проблема не в функции, а в том что такая реализация
// заставляет клиента писать небезопасный код.
//
// !! Функции мутаторы не должны возвр. по значению!!
template<class T>
T Stack<T>::Pop_nosafe() {
  if (vused_ == 0) {
    throw "if empty";
  } else {
    T result = v_[vused_];
    --vused_;
    return result;  // при копировании может возн. искл.
    // объект не дошел до плучателя, а состояние уже изменили
  }
}

template<class T>
void Stack<T>::Pop_better_but(T& result) {
  if (vused_ == 0) {
    throw "if empty";
  } else {
    result = v_[vused_];  // сперва снимаем значение
    --vused_;
    //return result;  // при копировании может возн. искл.
    // объект не дошел до плучателя, а состояние уже изменили
  }
}

template<class T>
T& Stack<T>::Top() {
  if (vused_ == 0) {
    throw "if empty";
  } else {
    return v_[vused_];
  }
}

template<class T>
void Stack<T>::Pop() {
  if (vused_ == 0) {
    throw "if empty";
  } else {
    --vused_;
  }
}

// А ведь и правда удаляет
int i = 0;
class TestThrow {
public:
  TestThrow() { i++;
                if (i == 5)
                  throw std::runtime_error(""); }
  ~TestThrow() {
      cout << "dtr\n";
  }
};


TEST(Sutter, Stackstrange) {
  EXPECT_THROW(Stack<TestThrow>(), std::runtime_error);// stack;
}

namespace util {
template<class T1, class T2>
void construct(T1* p, const T2& value) {
  new (p) T1(value);
}

template<class T>
void destroy(T* t) {
  t->~T();
}

template<class FwdIter>
void destroy(FwdIter first, FwdIter last) {
  while(first != last) {
      destroy(&*first);
      ++first;
  }
}
}

/// Updated Stack - step 6:
// Позволяет снять ограничения на T
// Нужен только конструктор копирования и nothrow ctor
template<class T>
class StackImpl {
/*?*/  // protected - if inh. public - member
protected:
//public:  // тогда можно будет включить членом класса, что много
  //  лучше!!
  StackImpl(size_t size=0);
  ~StackImpl();
  void Swap(StackImpl& other) throw();  // nothrow!! very important

  // видны дочернему - if protected - no
  T* v_;
  size_t vsize_;
  size_t vused_;

private:
  StackImpl(const StackImpl&);
  StackImpl& operator=(const StackImpl&);
};

template<class T>
StackImpl<T>::StackImpl(size_t size)
  : v_(static_cast<T*>
  (size == 0
      ? 0
  : operator new(sizeof(T)*size))),
       vsize_(size),
       vused_(0) {
}

template<class T>
StackImpl<T>::~StackImpl() {
  using util::destroy;

  destroy(v_, v_+vused_);  // compiled but...

  operator delete(v_);
}


template<class T>
void StackImpl<T>::Swap(StackImpl<T>& other) throw() {
  using std::swap;

  swap(v_, other.v_);  // а что будет потом с. Как деструктор определяет сколько раз вызвать дестр. объекта?
  swap(vsize_, other.vsize_);
  swap(vused_, other.vused_);
}

// Жестко связан с классом упр. ресурсами
// p. 125
template<class T>
class StackInh : private StackImpl<T> {
public:
  explicit StackInh(size_t size=0)
    : StackImpl<T>(size) {}

  //~StackInh();  // not need
  StackInh(const StackInh& other) {
    using util::construct;

    // gcc4.7.2 - no compiled without this->
    while (this->vused_ < other.vused_) {
        construct(this->v_+this->vused_, other.v_[this->vused_]);
        ++this->vused_;
    }
  }

  StackInh& operator=(const StackInh& other) {
    // не нужно проверять присв. самому себе
    StackInh temp(other);
    this->Swap(temp);
    return *this;
  }

  size_t Count() const {return this->vused_;}
  void Push(const T& t) {
    if (this->vused_ == this->vsize_) {
        // need reallocation
        StackInh temp(this->vsize_ * 2 +1);
        while (temp.Count() < this->vused_)
          temp.Push(this->v_[temp.Count()]);
        temp.Push(t);
        this->Swap(temp);
    } else {
      using util::construct;
        construct(this->v_ + this->vused_, t);
        ++this->vused_;
    }
  }
  T& Top();
  void Pop();
};

TEST(Sutter, StackFirst) {
  Stack<int> stack;
  StackInh<int> stackInh(10);
  StackInh<int> a(stackInh);
  a.Push(9);
  stackInh = a;
}

//TODO: часть 9

//TODO:
//Not-throw swap
// http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-throwing_swap
// http://www.gotw.ca/gotw/059.htm
// http://stackoverflow.com/questions/11485776/exception-safety-regarding-swap-operation-whats-so-wrong-with-that
// http://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom!!

//TODO:
// copy-and-swap idiom
// http://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom


//TODO: p. 339

// TODO: Exception on construction
// http://www.gotw.ca/gotw/066.htm
// http://herbsutter.com/2008/07/25/constructor-exceptions-in-c-c-and-java/
TEST(Sutter, ExcConstr) {
  {
    int(1);
    // http://www.informit.com/guides/content.aspx?g=cplusplus&seqNum=199
    const int& perch = int(1);

    // ... more code; at this point, only the first
    // temporary object is pushing up daisies ...
  }
  // Get it? It's a lifetime-of-temporaries-bound-to-references joke
}
