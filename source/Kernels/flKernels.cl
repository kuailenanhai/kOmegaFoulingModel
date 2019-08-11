__kernel __attribute__((reqd_work_group_size(WORKGROUPSIZE_FL_SHIFT, 1, 1)))
void Shift_walls(__global uint* __restrict__ BLdep,
	__global double* __restrict__ BLdep_tot,
	__global double2* __restrict__ C,
	__global double2* __restrict__ C0,
	const __global double* __restrict__ weightsL,
	const __global double* __restrict__ weightsR,
	__global uint* __restrict__ blInds,
	__global uint* __restrict__ cInds,
	__global double* __restrict__ flDisp,
	__global double2* __restrict__ flvN)
{
	int i = get_global_id(0);


	if (i >= FL_NUM_BL)
		return;

	uint BLstart = blInds[i];
	
	double4 WeightL = vloadn(i,weightsL);
	double4 WeightR = vloadn(i, weightsR);

	double disp = 0.;

	for (int j = 0; j < NUM_PAR_SIZES; j++)
	{
		uint CurBLstart = BLstart * NUM_PAR_SIZES + j;
		double8 Depf = convert_double8((uint8)(BLdep[CurBLstart], BLdep[CurBLstart + NUM_PAR_SIZES],
			BLdep[CurBLstart + 2 * NUM_PAR_SIZES], BLdep[CurBLstart + 3 * NUM_PAR_SIZES],
			BLdep[CurBLstart + 4 * NUM_PAR_SIZES], BLdep[CurBLstart + 5 * NUM_PAR_SIZES],
			BLdep[CurBLstart + 6 * NUM_PAR_SIZES], BLdep[CurBLstart + 7 * NUM_PAR_SIZES]));

		double disttemp = dot(Depf.lo, WeightL) + dot(Depf.hi, WeightR);
		disttemp += BLdep_tot[i + NUM_PAR_SIZES * j];
		BLdep_tot[i + NUM_PAR_SIZES + j] = disttemp;
		disp += (disttemp * Par_multiplier[j]);
	}
	flDisp[i] = disp;
	uint Cind = cInds[i];
	C[Cind] = C0[Cind] + disp * flvN[i];
}

__kernel __attribute__((reqd_work_group_size(WORKGROUPSIZE_FL_SHIFT, 1, 1)))
void Smooth_walls1(__global double* __restrict__ BLdep_tot,
	__global double* __restrict__ BLdep_tot2)
{
	int i = get_global_id(0);

	if (i >= FL_NUM_BL)
		return;

	int2 startstop = (i < FL_NUM_BL / 2) ? (int2(0, FL_NUM_ACTIVE_NODES/2)) : (int2(FL_NUM_ACTIVE_NODES/2, FL_NUM_ACTIVE_NODES));
	int kkstart = MAX(i - NEIGHS_PER_SIDE_SMOOTHING, startstop.x);
	int kkstop = MIN(i + 1 + NEIGHS_PER_SIDE_SMOOTHING, startstop.y);
	double num_locs = convert_double(kkstop - kkstart);
	for (int j = 0; j < NUM_PAR_SIZES; j++)
	{
		double Depf = 0.;
		int k = kkstart;
		while (k < kkstop)
		{
			Depf += BLdep_tot[k + NUM_PAR_SIZES * j] * PERCENT_USED_IN_SMOOTHING;
			k++;
		}
		BLdep_tot2[i + NUM_PAR_SIZES * j] = PERCENT_NOT_USED_IN_SMOOTHING * BLdep_tot[i + NUM_PAR_SIZES * j] + Depf / num_locs;
	}
}

__kernel __attribute__((reqd_work_group_size(WORKGROUPSIZE_FL_SHIFT, 1, 1)))
void Smooth_walls2(__global double* __restrict__ BLdep_tot,
	__global double2* __restrict__ C,
	__global double2* __restrict__ C0,
	__global uint* __restrict__ cInds,
	__global double* __restrict__ flDisp,
	__global double2* __restrict__ flvN)
{
	int i = get_global_id(0);

	if (i >= FL_NUM_BL)
		return;

	double disp = 0.;

	for (int j = 0; j < NUM_PAR_SIZES; j++)
	{
		disp += (BLdep_tot[i + NUM_PAR_SIZES * j] * Par_multiplier[j]);
	}

	flDisp[i] = disp;
	uint Cind = cInds[i];
	C[Cind] = C0[Cind] + disp * flvN[i];
}


__kernel __attribute__((reqd_work_group_size(WORKGROUPSIZE_FL_RAMP, 1, 1)))
void Ramp_ends(__global uint* __restrict__ IOinds,
	__global uint* __restrict__ Cinds,
	__global double* __restrict__ riYbegin,
	__global double* __restrict__ riCoeff,
	__global double2* __restrict__ C)
{
	int gid = get_global_id(0);

	if (gid >= WORKGROUPSIZE_FL_RAMP * 4)
		return;

	uint IOind = IOinds[gid];
	uint Cind = Cinds[gid];
	double Ycur = C[IOind].y;

	double Yshift = Ycur - riYbegin[gid];
	C[Cind].y = Ycur - riCoeff[gid] * Yshift;
}



