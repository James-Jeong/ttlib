#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strlib.h"

StringPtr NewString(char *s)
{
    if(s == NULL)
    {
        return NULL;
    }

    StringPtr newString = (String*)malloc(sizeof(String));
    if(newString == NULL)
    {
        return NULL;
    }
    
    size_t length = strlen(s);
    newString->data = (char*)malloc(length + 1);
    if(newString->data == NULL)
    {
        free(newString);
        return NULL;
    }

    memcpy(newString->data, s, length);
    *(newString->data + length) = '\0';

    newString->length = length;

    return newString;
}

void DeleteString(StringPtrContainer ptrContainer)
{
    free((*ptrContainer)->data);
    free(*ptrContainer);
    *ptrContainer = NULL;
}

StringPtr CloneString(StringPtr str)
{
    if(str == NULL)
    {
        return NULL;
    }

    size_t newLength = str->length;
    char *newData = (char*)malloc(newLength + 1);
    if(newData == NULL)
    {
        return NULL;
    }

    memcpy(newData, str->data, newLength);
    *(newData + newLength) = '\0';

    StringPtr newString = (String*)malloc(sizeof(String));
    if(newString == NULL)
    {
        free(newData);
        return NULL;
    }

    newString->data = newData;
    newString->length = newLength;

    return newString;
}

size_t GetLength(StringPtr str)
{
    if(str == NULL)
    {
        return 0;
    }

    return str->length;
}

char* GetPtr(StringPtr str)
{
    if(str == NULL)
    {
        return NULL;
    }

    return str->data;
}

char* SetString(StringPtr str, char *s)
{
    if(str == NULL || s == NULL)
    {
        return NULL;
    }

    size_t length = strlen(s);
    char *newData = (char*)malloc(length + 1);
    if(newData == NULL)
    {
        return NULL;
    }

    memcpy(newData, s, length);
    *(newData + length) = '\0';

    free(str->data);
    str->data = newData;
    str->length = length;

    return str->data;
}
