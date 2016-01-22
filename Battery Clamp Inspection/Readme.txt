1. Import the project onto Eclipse IDE. [File -> Import -> General -> Existing Projects into Workspace]
2. Build the Project,
	"Project Explorer->Battery Clamp Inspection->Build Configurations->Build all"
3. Copy the 'Images' folder and the executable('arm' or 'x64'\BatteryClampInspection for ARM and x64 Target respectively) to the same path on the target.
	Ex: /home/usr/BatteryClamInspection - executable,
		/home/usr/Images - Input Images.
4. Inspected images,
	a. x64 based target - Will be displayed on the Screen .
	b. armv7 based target - Will be written onto the the file system ("Images/InspectedImages/"