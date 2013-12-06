# coding: utf-8
import re

# http://habrahabr.ru/post/204476/

# Other
from generator.cpp import utils

# App
from _v8_glue_generators import make_array_getter, make_array_index_getter, make_scalars_and_accessors_with_formating
from _v8_glue_generators import make_getter_and_setter_add
from _v8_glue_generators import make_scalar_getter, make_scalar_setter
from _v8_glue_generators import make_array_index_setter
from _h_parser import Holder


if __name__ == '__main__':
    header_file_name = './test-data/han_test_.h'
    source = utils.ReadFile(header_file_name)

    type_and_var_list = Holder.extract_var_declaration(source)

    #for var in type_and_var_list:
    #    Indicator(*var)

    # такой будет вывод, когда подключим все массивы и функции
    if False:
        for elem in type_and_var_list:
            print(make_getter_and_setter_add(*elem))

        for elem in type_and_var_list:
            print(make_scalar_getter(*elem))
            print(make_scalar_setter(*elem))
    else:
        # временный вывод, где удалены пустые строки, в которых должны быть обернуты массивы
        # scalars and accessors in blueprint
        #print type_and_var_list
        #print(make_scalars_and_accessors_with_formating(type_and_var_list))
        # arrays
        #for elem in type_and_var_list:
        #    print(make_array_index_getter(*elem))
        #    print(make_array_index_setter(*elem))
        #    print(make_array_getter(*elem))
        pass

