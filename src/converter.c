#include "converter.h"

size_t byte_converter(size_t byte)
{
    // менее 1 кб - возвращаем число байт
    if (byte < KB)
        return byte;
    // меньше 1 мб - возвращаем число кб
    if (byte < MB)
        return byte / KB;
    // меньше 1 гб - возвращаем число мб
    if (byte < GB)
        return byte / MB;
    // больше или равен 1 гб - возвращаем число гб
    if (byte >= GB)
        return byte / GB;

    return -1;
};

char type_converter(size_t byte)
{
    // менее 1 кб - возвращаем тип байт
    if (byte < KB)
        return 'b';
    // меньше 1 мб - возвращаем тип кб
    if (byte < MB)
        return 'K';
    // меньше 1 гб - возвращаем тип мб
    if (byte < GB)
        return 'M';
    // больше или равен 1 гб - возвращаем тип гб
    if (byte >= GB)
        return 'G';

    return 'b';
};

size_t converter(Listnode* node)
{
    node->size_type = type_converter(node->byte);
    return byte_converter(node->byte);
};
