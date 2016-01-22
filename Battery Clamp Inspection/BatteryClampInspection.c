/*
 ============================================================================
 Name        : BatteryClampInspectionExample.c
 Author      : National Instruments
 Version     :
 ============================================================================
 */


/************************************************************************/
/*	Battery Clamp Inspection Example									*/
/*                    												    */
/*	This example loads a template image that is matched in each new 	*/
/*	image to locate the new coordinate system. Measurement areas are 	*/
/*	repositioned in the image according to the location of the 			*/
/*	coordinate system found. The example performs a distance 			*/
/*	measurement between two edges of the part inspected by using a 		*/
/*	clamp. The example performs a diameter measurement using a circular */
/*	edge finder.										 				*/
/*                    												    */
/*  IMAQ Vision functions used in this sample:						    */
/*		imaqClearOverlay												*/
/*		imaqCreateImage													*/
/*		imaqDisplayImage												*/
/*		imaqClampMax													*/
/*		imaqDispose														*/
/*		imaqFindCircularEdge											*/
/*		imaqFindTransformPattern										*/
/*		imaqMakePoint													*/
/*		imaqMoveWindow													*/
/*		imaqReadVisionFile												*/
/*		imaqReadFile													*/
/*		imaqSetWindowTitle												*/
/*																		*/
/************************************************************************/

#include "BatteryClampInspection.h"


// Global Variables

static int currentImage = 0;  /* track iterations of TimerLoop function */
static Image* BatteryImages[NUMIMAGES];  /* input images */
static Image* tempImage;
RotatedRect primaryRect = {110, 470, 190, 30, 0.00};  /* the rectangular search used in imaqFindTransformPattern */

Annulus searchArea;  /* circular search area used in imaqFindCircularEdge */

CoordinateTransform2 transform;  /* The coordinate transform the function updates based on the location and position of the object */

/* Options used in imaqFindTransfromPattern and imaqFindCircularEdge and imaqClampMax */
FindEdgeOptions ClampEdgeOptions = {40, 4, 2, 5, TRUE, TRUE, TRUE, FALSE};
FindEdgeOptions CircularEdgeOptions = {40, 4, 2, 10.00, TRUE, TRUE, TRUE, FALSE};
FindTransformPatternOptions TPOption = {IMAQ_MATCH_ROTATION_INVARIANT, 700.00, TRUE, NULL, 0,
			FALSE, FALSE, TRUE};

/****************************************************************************/
/*	The main function loads all of the images to be searched from disk 		*/
/*	and into array of images.  Next, the function defines the coordinates   */
/*	and the measurements. Will inspect the Loaded images.					*/
/****************************************************************************/
int main ()
{
	int i;
	char filename[MAX_PATHNAME_LEN];

	printf("\n ++ BatteryClampInspectionExample");

	// Read the input Images
	for( i = 0; i < NUMIMAGES; i++ )
	{
		BatteryImages[i] = imaqCreateImage( IMAQ_IMAGE_U8, 0 );
		sprintf(filename, "./Images/Image%02d.jpg", i);
		if(Log_Vision_Error(imaqReadFile(BatteryImages[i], filename, NULL, NULL)))
			goto cleanup;
	}

#if defined(__x86_64__)
	RTDisplayVideoMode displayMode = IMAQ_GRAPHICS_MODE;

	// Set the Display mode to Video
	if(Log_Vision_Error(imaqRTVideoMode(IMAQ_SET_MODE,&displayMode)))
		goto cleanup;
#endif

	// Define Measurements
	if(DefineCoordinate())
		goto cleanup;
	if(DefineMeasurements())
		goto cleanup;

	//Inspect the images
	Inspect();

cleanup:

#if defined(__x86_64__)
	// Set the Display mode back to Text
	displayMode = IMAQ_TEXT_MODE;
	Log_Vision_Error(imaqRTVideoMode(IMAQ_SET_MODE,&displayMode));
#endif

	/* After the program quits,we should dispose images to free system resources */
	for( i = 0; i < NUMIMAGES; i++ )
		imaqDispose(BatteryImages[i]);
	if(tempImage)
		imaqDispose(tempImage);

	printf("\n -- BatteryClampInspectionExample \n");

	return FALSE;
}

/*******************************************************************************/
/*	This function is used to compute the coordinate transform based on the	   */
/*	position of a pattern image.											   */
/*******************************************************************************/
int  DefineCoordinate ()
{
	int err = 0;

	tempImage = imaqCreateImage( IMAQ_IMAGE_U8, 0 );
	// Read the template Image
	err = Log_Vision_Error(imaqReadVisionFile (tempImage, "./Images/Template.png", NULL, NULL));
	if(err)
		goto cleanup;

	err = Log_Vision_Error(imaqClearOverlay(BatteryImages[0], NULL));
	if(err)
		goto cleanup;

	/* Computes a coordinate transform based on the position of a pattern image */
	/*	in a search area of an image											*/
	err = Log_Vision_Error(imaqFindTransformPattern(BatteryImages[0], tempImage, &transform,
					IMAQ_NO_ROTATED_RECT, IMAQ_FIND_REFERENCE, &TPOption, NULL));
	if(err)
		goto cleanup;

	err = Log_Vision_Error(DisplayInspectedImage(BatteryImages[0]));

cleanup:
	return err ? TRUE : FALSE ;
}

/*******************************************************************************/
/*	This function is used to define where to make the measurements on circular */
/*	edge.																	   */
/*******************************************************************************/
int  DefineMeasurements ()
{
	float distance;
	int err = 0;

	/* Define a search area for imaqFindCircularEdge */
	searchArea.center.x = 366;
	searchArea.center.y = 201;
	searchArea.innerRadius = 33;
	searchArea.outerRadius = 121;
	searchArea.startAngle = 42.71;
	searchArea.endAngle = 314.13;

	/* Measures a distance from the sides of the search area towards */
	/*	the center of the search area								 */
	err = Log_Vision_Error(imaqClampMax(BatteryImages[0], primaryRect,
			IMAQ_TOP_TO_BOTTOM, &distance, &ClampEdgeOptions, NULL, NULL, NULL));
	if(err)
		goto cleanup;

	/* Locates a circular edge in an annular search area */
	imaqFindCircularEdge(BatteryImages[0],searchArea, IMAQ_INSIDE_TO_OUTSIDE, &CircularEdgeOptions, NULL);

	// Display the Measurements on the Image
	err = Log_Vision_Error(DisplayInspectedImage(BatteryImages[0]));

cleanup:
	return err ? TRUE : FALSE;
}


/*************************************************************************/
/*  Inspect analyzes the images one by one and 						 	 */
/*  on x64 based Targets - Displays the image onto the screen            */
/*  on arm based Targets - Writes onto the file system.                  */
/*************************************************************************/
int  Inspect ()
{
	Image *CImage = NULL;
	float distance;
	int i = 0;
	int err = 0;

	/* set options to show the overlay results */
	ClampEdgeOptions.showResult = TRUE;
	CircularEdgeOptions.showResult = TRUE;

	/* Define a search area for imaqFindCircularEdge */
	searchArea.center.x = 366;
	searchArea.center.y = 201;
	searchArea.innerRadius = 33;
	searchArea.outerRadius = 121;
	searchArea.startAngle = 42.71;
	searchArea.endAngle = 314.13;

	/* Set image to point to the current image */
	for( i = 0; i < NUMIMAGES; i++ )
	{
		CImage = BatteryImages[currentImage];

		/* Clear any overlay on this image and display it */
		err = Log_Vision_Error(imaqClearOverlay(CImage, NULL));
		if(err)
			goto cleanup;

		/* Computes a coordinate transform based on the position of a pattern image */
		/*	in a search area of an image											*/
		err = Log_Vision_Error(imaqFindTransformPattern(CImage, tempImage,
				&transform, IMAQ_NO_ROTATED_RECT, IMAQ_UPDATE_TRANSFORM, &TPOption, NULL));
		if(err)
			goto cleanup;

		/* Measures a distance from the sides of the search area towards */
		/*	the center of the search area								 */
		err = Log_Vision_Error(imaqClampMax(CImage, primaryRect,
				IMAQ_TOP_TO_BOTTOM, &distance, &ClampEdgeOptions, &transform, NULL, NULL));
		if(err)
			goto cleanup;

		/* Locates a circular edge in an annular search area */
		imaqFindCircularEdge(CImage, searchArea, IMAQ_INSIDE_TO_OUTSIDE, &CircularEdgeOptions, &transform);

		// Display the inspected image
		err = Log_Vision_Error(DisplayInspectedImage(CImage));

		currentImage++;
	}

cleanup:
	return err ? TRUE : FALSE;
}

/*************************************************************************/
/*  DisplayInspectedImage :											 	 */
/*  on x64 based Targets - Displays the image onto the screen            */
/*  on arm based Targets - Writes onto the file system.                  */
/*************************************************************************/
int DisplayInspectedImage(Image* image)
{

	int err = 0;

#if defined(__x86_64__)

	//Display the Image
	err = Log_Vision_Error(imaqRTDisplayImage(image, NULL, 0));

	sleep(2); // Wait for 2 seconds before the image unload

#else
	// Write the Overlay Image to the file (Display not supported by the Target)
	// Output Image will be created in the Images folder(copied onto the Target)
	static int outputImage = 0;
	Image* outputOverlayImage = NULL;
	char filename[MAX_PATHNAME_LEN];

	outputOverlayImage = imaqCreateImage (IMAQ_IMAGE_RGB, 0);
	sprintf(filename, "./Images/InspectedImages/Image%02d.jpg", outputImage++);
	err = Log_Vision_Error(imaqMergeOverlay(outputOverlayImage, image,0,0,0));
	if(err)
		goto cleanup;
	err = Log_Vision_Error(imaqWriteFile(outputOverlayImage, filename, NULL));

cleanup:
	imaqDispose(outputOverlayImage);

#endif

	return err ? TRUE : FALSE;
}

// Print the VISION API Error Message
int Log_Vision_Error(int errorValue)
{
	if ( (errorValue != TRUE) && (imaqGetLastError() != ERR_SUCCESS)) {
		char *tempErrorText = imaqGetErrorText(imaqGetLastError());
		printf("\n %s ", tempErrorText);
		imaqDispose(tempErrorText);
		return TRUE;
	}
	return FALSE;
}

