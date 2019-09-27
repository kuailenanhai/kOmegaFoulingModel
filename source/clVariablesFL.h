// clVariablesFL.h: Fouling layer class.
//
// (c) Zach Mills, 2015 
//////////////////////////////////////////////////////////////////////

//TODO: need to save necessary variables (both as txt and bin files)
//	need to clean up unncessary functions and variables and comment
//	need to set up load functions for restarting simulation

#if !defined(AFX_CLVARIABLESFL_H__INCLUDED_)
#define AFX_CLVARIABLESFL_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include "Kernels.h"
#include "Array.h"
#include "clProblem.h"
#include "foulStructs.h"
#include "clVariablesTR.h"

class clVariablesTR;

class clVariablesFL
{
public:
		   
	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////	
	//////////////                                               ///////////////
	//////////////    CONSTRUCTOR/DESTRUCTOR/ENUMS/FUNC_PTRS     ///////////////
	//////////////                                               ///////////////
	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////


	// Func Pointer for calling loadParams
	std::function<void(void)> loadParamsPtr;


	clVariablesFL() : blDepTot("blDepTot"), blDepTot_temp("blDepTotTemp"),
		IO_ind_dist("ioIndDist"), FI("FoulI"), RI("RampI")
	{
		loadParamsPtr = std::bind(&clVariablesFL::loadParams, this);
	};


	virtual ~clVariablesFL()
	{
	};




	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	//////////////                                               ///////////////
	//////////////           KERNELS/REDUCTIONS/SOLVERS          ///////////////
	//////////////                                               ///////////////
	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	Kernel shiftWallsKernel, smoothWallsKernel[2], rampEndsKernel;



	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////	
	//////////////                                               ///////////////
	//////////////                   ARRAYS                      ///////////////
	//////////////                                               ///////////////
	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	
	foulI FI;
	rampI RI;
	Array2Dd blDepTot, blDepTot_temp;
	Array1Dd IO_ind_dist;
	
	//Array1Dd Sum_M_temp;
	//Array1Dd Debug_out;
	
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

	bool flSolverFlag, saveOnStartFlag;
	bool restartRunFlag;
	cl_uint4 IO_end;
	cl_uint Num_active_nodes;
	cl_uint Num_IO_indicies;
	int FL_timer, flTimePerUpdate;
	int Smooth_timer, flTimePerSmooth, neighsPerSideSmoothing;
	int SaveStepNum = 0;
	double smoothingPct;
	int updateTRActiveFreq;
	statKernelType kernelT;
	double mindXDist;

	////////////////////////////////////////////////////////////////////////////	
	//////////////             Run Parameter Variables           ///////////////
	////////////////////////////////////////////////////////////////////////////	


	////////////////////////////////////////////////////////////////////////////	
	//////////////                Method Variables               ///////////////
	////////////////////////////////////////////////////////////////////////////	




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

	// initialization function for time data array
	void iniTimeData();

	// Loads parameters passed in yaml parameter file, (also reads in 
	// restart variables when a run is restarted)
	void loadParams();

	// Copies saved files from main folder into results folder to ensure
	// that next files do not save 
	void renameSaveFiles();

	// Writes output data to file(specific arrays, not all of them)
	void save2file();

	// Writes additional data to file for debugging purposes
	void saveDebug();

	// Saves parameters to yaml file used for restarting runs
	void saveParams();

	// Saves bin files necessary to restart run
	void saveRestartFiles();

	// Saves time output data (i.e. avg velocity, shear, Nu, etc)
	void saveTimeData();

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
	void updateTimeData();
	   	 

	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////	
	//////////////                                               ///////////////
	//////////////            CLASS SPECIFIC FUNCTIONS           ///////////////
	//////////////                                               ///////////////
	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////

	
	cl_double2 get_center(cl_double2 P0, cl_double2 P1);
	void testFlUpdate();
	
	////////////////////////////////////////////////////////////////////////////	
	//////////////            Initialization Functions           ///////////////
	////////////////////////////////////////////////////////////////////////////

	void getNumActiveNodes();
	void iniFoulI();
	void iniIOVars();//done


	////////////////////////////////////////////////////////////////////////////	
	//////////////              Updating Functions               ///////////////
	////////////////////////////////////////////////////////////////////////////

	void update();
	void updateFL();

	// Updates arrays used in the calculation of shear stress at boundary nodes
	// and, following this, at boundary links
	void updateShearArrays();

	////////////////////////////////////////////////////////////////////////////	
	//////////////               Solving Functions               ///////////////
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////	
	//////////////                Output Functions               ///////////////
	////////////////////////////////////////////////////////////////////////////

	void saveVariables();

	////////////////////////////////////////////////////////////////////////////	
	//////////////                Display Functions              ///////////////
	////////////////////////////////////////////////////////////////////////////


	//void ini_group_sizes();
	//
	////saves variables necessary for debugging
	//bool test_bounds();
	//bool Restart_Run();
	//void UpdateRestart();
	//void RenameDebug_Files(int dirnumber);
	//void CallRename(char* file, const char* fol);













};


#endif