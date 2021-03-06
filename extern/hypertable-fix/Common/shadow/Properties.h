/*
 * Copyright (C) 2007-2012 Hypertable, Inc.
 *
 * This file is part of Hypertable.
 *
 * Hypertable is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or any later version.
 *
 * Hypertable is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

/** @file
 * Program options handling.
 * Based on boost::program_options, the options are configured in Config.h.
 */

#ifndef HYPERTABLE_PROPERTIES_H
#define HYPERTABLE_PROPERTIES_H

#include <vector>
#include <string>
#include <boost/version.hpp>
#include <boost/any.hpp>

// Required declarations for custom validators *before* including the header,
// otherwise no overloading would happen in standard conforming compilers.

/** Boost library. */
namespace boost {

/** Program options. */
namespace program_options {

typedef std::vector<std::string> Strings;
typedef std::vector<int64_t> Int64s;

void validate(boost::any &v, const Strings &values, ::int64_t *, int);
void validate(boost::any &v, const Strings &values, ::int32_t *, int);
void validate(boost::any &v, const Strings &values, ::uint16_t *, int);
void validate(boost::any &v, const Strings &values, double *, int);

// pre 1.35 vector<T> validate doesn't pickup user defined validate for T
#if BOOST_VERSION < 103500
template<typename T>
void validate(boost::any& v, const Strings &s, std::vector<T>*, int);
#endif

}} // namespace boost::program_options

#include <boost/program_options.hpp>

#include "Common/ReferenceCount.h"
#include "Common/Error.h"

namespace boost { namespace program_options {

#if BOOST_VERSION < 103500
/** Implement validation function for vector<T>, which is not
 * implemented in boost prior to 1.35
 */
template<typename T>
void validate(boost::any& v, const Strings &s, std::vector<T>*, int) {
  if (v.empty())
      v = boost::any(std::vector<T>());

  std::vector<T>* tv = boost::any_cast< std::vector<T> >(&v);
  assert(NULL != tv);

  for (unsigned i = 0; i < s.size(); ++i) {
    try {
      // so we can pick up user defined validate for T here
      boost::any a;
      Strings sv;
      sv.push_back(s[i]);
      validate(a, sv, (T*)0, 0);
      tv->push_back(boost::any_cast<T>(a));
    }
    catch (const bad_lexical_cast &/*e*/) {
      boost::throw_exception(invalid_option_value(s[i]));
    }
  }
}
#endif // < boost 1.35

}} // namespace boost::program_options

// convenience/abbreviated accessors
#define HT_PROPERTIES_ABBR_ACCESSORS(_const_) \
  inline bool get_bool(const String &name) _const_ { \
    return get<bool>(name); } \
  inline String get_str(const String &name) _const_ { \
    return get<String>(name); } \
  inline Strings get_strs(const String &name) _const_ { \
    return get<Strings>(name); } \
  inline uint16_t get_i16(const String &name) _const_ { \
    return get<uint16_t>(name); } \
  inline int32_t get_i32(const String &name) _const_ { \
    return get<int32_t>(name); } \
  inline int64_t get_i64(const String &name) _const_ { \
    return get<int64_t>(name); } \
  inline Int64s get_i64s(const String &name) _const_ { \
    return get<Int64s>(name); } \
  inline double get_f64(const String &name) _const_ { \
    return get<double>(name); } \
  inline Doubles get_f64s(const String &name) _const_ { \
    return get<Doubles>(name); } \
  inline bool get_bool(const String &name, bool default_value) _const_ { \
    return get(name, default_value); } \
  inline String get_str(const String &name, const String &default_value) \
    _const_ { return get(name, default_value); } \
  inline Strings get_strs(const String &name, const Strings &default_value) \
    _const_ { return get(name, default_value); } \
  inline uint16_t get_i16(const String &name, uint16_t default_value) \
    _const_ { return get(name, default_value); } \
  inline int32_t get_i32(const String &name, int32_t default_value) _const_ { \
    return get(name, default_value); } \
  inline int64_t get_i64(const String &name, int64_t default_value) _const_ { \
    return get(name, default_value); } \
  inline Int64s get_i64s(const String &name, const Int64s &default_value) \
    _const_ { return get(name, default_value); } \
  inline double get_f64(const String &name, double default_value) _const_ { \
    return get(name, default_value); } \
  inline Doubles get_f64s(const String &name, const Doubles &default_value) \
    _const_ { return get(name, default_value); }

namespace Hypertable {

/** @addtogroup Common
 *  @{
 */

namespace Po = boost::program_options;

typedef std::vector<String> Strings;
typedef std::vector<int64_t> Int64s;
typedef std::vector<double> Doubles;

namespace Property {

const uint64_t K = 1000;
const uint64_t KiB = 1024;
const uint64_t M = K * 1000;
const uint64_t MiB = KiB * 1024;
const uint64_t G = M * 1000;
const uint64_t GiB = MiB * 1024;

// Abbrs for some common types
inline Po::typed_value<bool> *boo(bool *v = 0) {
  return Po::value<bool>(v);
}

inline Po::typed_value<String> *str(String *v = 0) {
  return Po::value<String>(v);
}

inline Po::typed_value<Strings> *strs(Strings *v = 0) {
  return Po::value<Strings>(v);
}

inline Po::typed_value<uint16_t> *i16(uint16_t *v = 0) {
  return Po::value<uint16_t>(v);
}

inline Po::typed_value<int32_t> *i32(int32_t *v = 0) {
  return Po::value<int32_t>(v);
}

inline Po::typed_value<int64_t> *i64(int64_t *v = 0) {
  return Po::value<int64_t>(v);
}

inline Po::typed_value<Int64s> *i64s(Int64s *v = 0) {
  return Po::value<Int64s>(v);
}

inline Po::typed_value<double> *f64(double *v = 0) {
  return Po::value<double>(v);
}

inline Po::typed_value<Doubles> *f64s(Doubles *v = 0) {
  return Po::value<Doubles>(v);
}

} // namespace Property

typedef Po::options_description PropertiesDesc;
typedef Po::positional_options_description PositionalDesc;

/**
 * Manages a collection of program options.
 *
 * Implements getters and setters, aliases and default values.
 */
class Properties : public ReferenceCount {
  typedef Po::variable_value Value;
  typedef Po::variables_map Map;
  typedef std::pair<Map::iterator, bool> InsRet;
  typedef std::map<String, String> AliasMap;

public:
  /** Default constructor; creates an empty set */
  Properties()
    : m_need_alias_sync(false) {
  }

  /** Constructor; load properties from a filename
   *
   * @param filename The name of the file with the properties
   * @param desc A Property description with valid properties, default values
   * @param allow_unregistered If true, unknown/unregistered properties are
   *        accepted
   */
  Properties(const String &filename, const PropertiesDesc &desc,
             bool allow_unregistered = false)
      : m_need_alias_sync(false) {
    load(filename, desc, allow_unregistered);
  }

  /**
   * Loads a configuration file with properties
   *
   * @param filename The name of the configuration file
   * @param desc A property description
   * @param allow_unregistered If true, unknown/unregistered properties are
   *        accepted
   */
  void load(const String &filename, const PropertiesDesc &desc,
          bool allow_unregistered = false);

  /**
   * Parses command line arguments
   *
   * @param argc The argument count (from main)
   * @param argv The argument array (from main)
   * @param desc The options description
   * @param hidden The hidden options description
   * @param p The positional options description
   * @param allow_unregistered If true, unknown/unregistered properties are
   *        accepted
   * @throw Error::CONFIG_INVALID_ARGUMENT on error
   */
  void parse_args(int argc, char *argv[], const PropertiesDesc &desc,
             const PropertiesDesc *hidden = 0, const PositionalDesc *p = 0,
             bool allow_unregistered = false);

  /**
   * Parses command line arguments
   *
   * @param args A vector of Strings with the command line arguments
   * @param desc The options description
   * @param hidden The hidden options description
   * @param p The positional options description
   * @param allow_unregistered If true, unknown/unregistered properties are
   *        accepted
   * @throw Error::CONFIG_INVALID_ARGUMENT on error
   */
  void parse_args(const std::vector<String> &args, const PropertiesDesc &desc,
             const PropertiesDesc *hidden = 0, const PositionalDesc *p = 0,
             bool allow_unregistered = false);

  /**
   * Calls user-defined notifier functions (if any) with final values
   */
  void notify();

  /**
   * Get the value of option of type T. Throws if option is not defined.
   *
   * @param name The name of the property
   * @throw Error::CONFIG_GET_ERROR if the requested property is not defined
   */
  template <typename T>
  T get(const String &name) const {
    try {
      return m_map[name].template as<T>();
    }
    catch (std::exception &e) {
      HT_THROWF(Error::CONFIG_GET_ERROR, "getting value of '%s': %s",
                name.c_str(), e.what());
    }
  }

  /**
   * Get the value of option of type T. Returns supplied default value if
   * not found. Try use the first form in usual cases and supply default
   * values in the config descriptions, as it validates the name and is less
   * error prone.
   *
   * @param name The name of the property
   * @param default_value The default value to return if not found
   */
  template <typename T>
  T get(const String &name, const T &default_value) const {
    try {
      Map::const_iterator it = m_map.find(name);

      if (it != m_map.end())
        return (*it).second.template as<T>();

      return default_value;
    }
    catch (std::exception &e) {
      HT_THROWF(Error::CONFIG_GET_ERROR, "getting value of '%s': %s",
                name.c_str(), e.what());
    }
  }

  /**
   * Get the underlying boost::any value of 'name'
   *
   * @param name The name of the property
   */
  const boost::any &operator[](const String &name) const {
    return m_map[name].value();
  }

  /**
   * Check whether a property has a default value
   *
   * @param name The name of the property
   * @return true if the value is default
   */
  bool defaulted(const String &name) const {
    return m_map[name].defaulted();
  }

  /** Check whether a property exists
   *
   * @param name The name of the property
   * @return true if the property exists
   */
  bool has(const String &name) const {
    return m_map.count(name);
  }

  HT_PROPERTIES_ABBR_ACCESSORS(const)

  /**
   * Add property to the map
   *
   * @param name The name of the property
   * @param v The value of the property
   * @param defaulted True if the value is default
   * @return An iterator to the new item
   */
  InsRet add(const String &name, const boost::any &v, bool defaulted = false) {
    m_need_alias_sync = true;
    return m_map.insert(Map::value_type(name, Value(v, defaulted)));
  }

  /**
   * Set a property in the map, create if not found
   *
   * @param name The name of the property
   * @param v The value of the property
   * @param defaulted True if the value is default
   */
  void set(const String &name, const boost::any &v, bool defaulted = false) {
    InsRet r = add(name, v, defaulted);

    if (!r.second)
      (*r.first).second = Value(v, defaulted);
  }

  /**
   * Remove a property from the map
   *
   * @param name The name of the property
   */
  void remove(const String &name) {
    m_map.erase(name);
  }

  /**
   * Setup an alias for a property.
   *
   * The primary property has higher priority, meaning when
   * aliases are sync'ed the primary value can override secondary value
   *
   * @param primary The primary property name
   * @param secondary The secondary property name
   * @param overwrite True if an existing alias should be overwritten
   */
  void alias(const String &primary, const String &secondary,
          bool overwrite = false);

  /**
   * Sync alias values.
   *
   * After this operation all properties that are aliases to each other
   * have the same value. Value priority: primary non-default >
   * secondary non-default > primary default > secondary default
   */
  void sync_aliases();

  /**
   * Returns all property names
   *
   * @param names reference to vector to hold names of all properties
   */
  void get_names(std::vector<String> &names) {
    for (Map::const_iterator it = m_map.begin(); it != m_map.end(); it++)
      names.push_back((*it).first);
  }

  /**
   * Prints keys and values of the configuration map
   *
   * @param out The output stream
   * @param include_default If true then default values are included
   */
  void print(std::ostream &out, bool include_default = false);

  /**
   * Helper to print boost::any used by property values
   *
   * @param a Reference to the boost::any value
   * @return A string with the formatted value
   */
  static String to_str(const boost::any &a);

private:
  /** Whether the aliases need to be synced */
  bool m_need_alias_sync;

  /** The map containing all properties */
  Map m_map;

  /** A map with all aliases */
  AliasMap m_alias_map;
};

typedef intrusive_ptr<Properties> PropertiesPtr;

/** Helper class to access parts of the properties.
 *
 * This snippet extracts all properties that start with
 * "Hypertable.RangeServer." and uses the SubProperty to access
 * "Hypertable.RangeServer.Range.SplitSize". 
 *
 *     SubProperties cfg(props, "Hypertable.RangeServer.");
 *     i64_t foo = cfg.get_i64("Range.SplitSize");
 */
class SubProperties {
public:
  /** Constructor
   *
   * @param props The original Properties object
   * @param prefix The common prefix of all sub-properties
   */
  SubProperties(PropertiesPtr &props, const String &prefix)
    : m_props(props), m_prefix(prefix) {
  }

  /** Returns the full name of a sub-property
   *
   * @param name The name of the sub-property
   * @return The full name (prefix + name)
   */
  String full_name(const String &name) const {
    return m_prefix + name;
  }

  /**
   * Calls Properties::get for a sub-property.
   *
   * @param name The name of the sub-property
   */
  template <typename T>
  T get(const String &name) const {
    return m_props->get<T>(full_name(name));
  }

  /**
   * Calls Properties::get for a sub-property.
   *
   * @param name The name of the sub-property
   * @param default_value The default value to return if not found
   */
  template <typename T>
  T get(const String &name, const T &default_value) const {
    return m_props->get(full_name(name), default_value);
  }

  /**
   * Check whether a sub-property has a default value
   *
   * @param name The name of the sub-property
   * @return true if the value is default
   */
  bool defaulted(const String &name) const {
    return m_props->defaulted(full_name(name));
  }

  /**
   * Check whether a sub-property exists
   *
   * @param name The name of the sub-property
   * @return true if the property exists
   */
  bool has(const String &name) const {
    return m_props->has(full_name(name));
  }

  HT_PROPERTIES_ABBR_ACCESSORS(const)

private:
  /** Reference to the original Properties object */
  PropertiesPtr m_props;

  /** The common prefix of all sub-properties */
  String m_prefix;
};

/** @} */

} // namespace Hypertable

#endif // HYPERTABLE_PROPERTIES_H
