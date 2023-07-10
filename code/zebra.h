#ifndef CODE_ZEBRA_H_
#define CODE_ZEBRA_H_

#include "zf_common_headfile.h"

enum Zebra_status{
    ZEBRA_IN = 30,
    ZEBRA_NONE,
};

extern int garage_flag;

void Startline_process(Trait_smachine *road_smh, unsigned char (*binary_array)[188]);


#endif /* CODE_ZEBRA_H_ */
