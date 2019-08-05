// particleDisplay.h: Class containing variables and functions used
// to make openGL calls which display particles, as well as walls
// on the screen.
//
// (c) Zachary Mills, 2019 
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTICLEDISPLAY_H__INCLUDED_)
#define AFX_PARTICLEDISPLAY_H__INCLUDED_

#pragma once

#include "StdAfx.h"
#include "Kernels.h"
#include "Array.h"
#include "particleStructs.h"



class particleDisplay
{
public:
	
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////	
//////////////                                               ///////////////
//////////////    CONSTRUCTOR/DESTRUCTOR/ENUMS/FUNC_PTRS     ///////////////
//////////////                                               ///////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

	particleDisplay() : P_vbo("pVBO"), parColorList("parColorList"),
		Tcrit_color("TcritColor"), LSt_vbo("LSt_vbo"),
		LSb_vbo("LSb_vbo"), LSt0_vbo("LSt0_vbo"), LSb0_vbo("LSb0_vbo"),
		LinesV("LinesV"), LinesH("LinesH")
	{}

	~particleDisplay()
	{}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//////////////                                               ///////////////
//////////////           KERNELS/REDUCTIONS/SOLVERS          ///////////////
//////////////                                               ///////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

	// Updates P_vbo before updating display with opengl calls
	Kernel TR_GL_kernel;


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////	
//////////////                                               ///////////////
//////////////                   ARRAYS                      ///////////////
//////////////                                               ///////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////	
//////////////                   Data Arrays                 ///////////////
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////	
//////////////                   Method Arrays               ///////////////
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////	
//////////////                   Output Arrays               ///////////////
////////////////////////////////////////////////////////////////////////////	

////////////////////////////////////////////////////////////////////////////	
//////////////                   Display Arrays              ///////////////
////////////////////////////////////////////////////////////////////////////
	
	// Array of points for tracers
	Array1DGL P_vbo;

	// Array containing colors corresponding to tracers
	Array1Df parColorList;

	// 2 element array containing the max shear stress values for
	// the metal and fouling layer interfaces
	Array1Dd Tcrit_color;

	// Current Position of the Walls
	Array1DGL LSt_vbo, LSb_vbo;   

	// Original Position of the Walls
	Array1DGL LSt0_vbo, LSb0_vbo; 	
	
	//Gridlines (Uncomment OPENGL_GRIDLINES to use)
	Array1DGL LinesV, LinesH;	  

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////	
//////////////                                               ///////////////
//////////////                 OPENCL BUFFERS                ///////////////
//////////////                                               ///////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////	
//////////////                                               ///////////////
//////////////                   VARIABLES                   ///////////////
//////////////                                               ///////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////	
//////////////             Run Parameter Variables           ///////////////
////////////////////////////////////////////////////////////////////////////	
	
	int numParGL;		// number of particles to display with opengl
	int OpenGL_timer;	// frequency that opengl is called
	int numParPerGLObj; // number of tracers each gl object represents
	bool glGridFlag;	// flag indicating if gridlines should be shown
	double pointSizes;	// initial size of particles
	double lineSizes;	// initial thickness of lines
	int glScreenWidth;	// width of opengl window
	int glScreenHeight; // height of opengl window

////////////////////////////////////////////////////////////////////////////	
//////////////                Method Variables               ///////////////
////////////////////////////////////////////////////////////////////////////	

	int numLinesV, numLinesH; // number of verticle and horizontal gridlines


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////	
//////////////                                               ///////////////
//////////////                  BASE FUNCTIONS               ///////////////
//////////////                                               ///////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////	


	// Allocates host arrays containing data
	void allocateArrays();

	// Allocates device buffers, and copies host contents to them
	void allocateBuffers();

	// Creates kernels from compiled openCL source code. Pointers to 
	// these functions are passed to sourceGenerator to allow for 
	// them to be called after compilation
	void createKernels();

	// Frees arrays on host which are no longer needed to save memory
	void freeHostArrays();

	// Inititialization function
	void ini();

	// Loads parameters passed in yaml parameter file, (also reads in 
	// restart variables when a run is restarted)
	void loadParams();

	// Copies saved files from main folder into results folder to ensure
	// that next files do not save 
	void renameSaveFiles() {};

	// Writes output data to file(specific arrays, not all of them)
	void save2file() {};

	// Writes additional data to file for debugging purposes
	void saveDebug() {};

	// Saves parameters to yaml file used for restarting runs
	void saveParams();

	// Saves bin files necessary to restart run
	void saveRestartFiles() {};

	// Saves time output data (i.e. avg velocity, shear, Nu, etc)
	void saveTimeData() {};

	// Sets arguments to kernels.Pointer to this function is passed to 
	// sourceGenerator along with createKernels so that after kernels 
	// are created, all arguments are set.
	// Note: parameters cannot be set in any other initialization functions,
	// because kernel creation is last initialization step.
	void setKernelArgs();

	// Prepends macro definitions specific to the class method to opencl source
	void setSourceDefines();

	// Tests to see if run is new, or if it is a restart.If information is 
	// missing, the run cannot restart.
	// Note: The run will be able to load temp and velocity data while still
	// being a new run for remaining methods
	bool testRestartRun();

	// Calls kernels to save time data (umean, avg density, etc) to array
	// on device, which will eventually be saved if it reaches its max 
	// size, or a save step is reached
	void updateTimeData() {};


	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////	
	//////////////                                               ///////////////
	//////////////            CLASS SPECIFIC FUNCTIONS           ///////////////
	//////////////                                               ///////////////
	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////	
	//////////////            Initialization Functions           ///////////////
	////////////////////////////////////////////////////////////////////////////


	// initializes arrays corresponding to boundary links
	void iniBLArrays();
	
	// initializes arrays corresponding to gridlines.
	void iniGridLines();

	// initializes arrays corresponding to particles
	void iniParticleColors();
	
	
	////////////////////////////////////////////////////////////////////////////	
	//////////////              Updating Functions               ///////////////
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////	
	//////////////               Solving Functions               ///////////////
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////	
	//////////////                Output Functions               ///////////////
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////	
	//////////////                Display Functions              ///////////////
	////////////////////////////////////////////////////////////////////////////


};














#endif