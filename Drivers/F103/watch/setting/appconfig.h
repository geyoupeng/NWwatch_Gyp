#ifndef __APPCONFIG_H
#define __APPCONFIG_H

#include "common.h"
#include <stdbool.h>
#include <stdint.h>

extern appconfig_s appConfig;
void AppConfigSave(void);
void AppConfigInit(void);
void AppConfigReset(void);

#endif 
