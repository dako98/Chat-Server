#ifndef __CODES_H__
#define __CODES_H__

enum StatusCodes
{
    INVALID_CODE = -1,

    OK,
    
    WRONG_SENDER    = 1 << 0,
    WRONG_RECEIVER  = 1 << 1,
    WRONG_CONTENTS  = 1 << 2,

    TERMINATED = WRONG_SENDER | WRONG_RECEIVER | WRONG_CONTENTS,

    COUNT
};

#endif // __CODES_H__
