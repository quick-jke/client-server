#ifndef RESULT_CODE_HPP
#define RESULT_CODE_HPP

enum ResultCode     // коды завершения файловых операций
{
    R_OK,              // успешное завершение
    R_REPEAT,          // успешное неполное завершение
    R_INFORMATION,     // информация
    R_FILE_NOT_FOUND,  // ошибка - файл не найден
    R_BADFILESTRUCT,   // ошибка - некорректная структура данных
    R_OUTOFMEMORY,     // ошибка - невозможно выделить память
    R_OPENERROR,       // ошибка - open
    R_CLOSEERROR,      // ошибка - close
    R_READERROR,       // ошибка - read
    R_WRITEERROR,      // ошибка - write
    R_RENAMEERROR,     // ошибка - rename
    R_REMOVEERROR,     // ошибка удаления
    R_ENDFILE,         // ошибка - конец файла
    R_INVALIDOBJECT,   // некорректный объект
    R_INVALIDINDEX,    // некорректный индекс
    R_DUPLICATEDINDEX, // дублирующий индекс
    R_NODISKSPACE,     // нет места на диске
    R_OBJECTSERROR,    // ошибка загрузки объектов
    R_MAPNOTOPEN,      // ошибка - доступ к неоткрытой карте 
    R_NOIDENT,         // ошибка - нет объекта с указанным идентификатором
    R_CHANGED,         // объект изменен
    R_FROZEN,          // объект заморожен
};


#endif
