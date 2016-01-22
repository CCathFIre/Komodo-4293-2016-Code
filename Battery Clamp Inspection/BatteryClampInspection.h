/*
 ============================================================================
 Name        : BatteryClampInspectionExample.h
 Author      : National Instruments
 Version     :
 ============================================================================
 */

// Vision Includes
#include <nivision.h>
#include <nimachinevision.h>


/* constants used in this example */
#define	TRUE	1
#define FALSE	0
#define	NUMIMAGES	17		/* number of images stored on disk */
#define	MAX_PATHNAME_LEN	1024

int	DefineCoordinate();
int	DefineMeasurements();
int Inspect();
int	Log_Vision_Error(int errorValue);

// Display supported only on x64 based Targets
int	DisplayInspectedImage(Image* image);
