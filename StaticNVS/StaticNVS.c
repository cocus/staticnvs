#include "snvs.h"
#include "snvs_generic_file.h"

#include <stdio.h>
#include <tchar.h>











/* Define functions that call fopen, fseek, fread, fwrite as a generic callbacks for SNVS */
SNVS_GENERIC_FILE_IO
(
	master_cfg,							/* Prefix for the functions */
	"master_cfg.bin"					/* File name */
);


typedef struct
{
	uint8_t			config1_u8;
	char			config1_string[30];
} myConfig1;

typedef struct
{
	uint16_t		config2_u16;
	char			config2_string[30];
} myConfig2;

#define MASTER_CFG(d)		d(myConfig1) d(myConfig2)

SNVS_SPAWN_GLOBAL_CONFIG
(
	MASTER_CFG,							/* Macro with configuration struct names */
	0,									/* Offset where this configuration should reside */
	master_cfg_io,						/* IO callback */
	master_cfg_write,					/* Write callback */
	master_cfg_read						/* Read callback */
);


int main(int argc, char** argv)
{
	//SNVS_SPAWN_LOCAL_VARIABLES(MASTER_CFG);

	MASTER_CFG_RECALL();//&MASTER_CFG_struct);

	printf("Sizeof MASTER_CFG: %d\n Sizeof myConfig1: %d\n Sizeof myConfig2: %d\n", SNVS_SIZEOF(MASTER_CFG), sizeof(myConfig1), sizeof(myConfig2));
	snprintf(_MASTER_CFG.myConfig1_.config1_string, sizeof(_MASTER_CFG.myConfig1_.config1_string), "String de prueba 1!");
	snprintf(_MASTER_CFG.myConfig2_.config2_string, sizeof(_MASTER_CFG.myConfig2_.config2_string), "String de prueba 2!");

	MASTER_CFG_STORE();//&MASTER_CFG_struct);

    return 0;
}