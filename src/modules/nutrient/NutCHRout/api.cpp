/*!
 * \file api.cpp
 * \brief Define MetadataInfo of NutCHRout module.
/*!
 * \file api.cpp
 * \ingroup NutCHRout
 * \author Huiran Gao
 * \date Jun 2016
 */


#include <stdio.h>
#include <string>
#include "api.h"
#include "util.h"
#include "NutrientCHRoute.h"
#include <iostream>
#include "SimulationModule.h"
#include "MetadataInfo.h"
#include "MetadataInfoConst.h"

extern "C" SEIMS_MODULE_API SimulationModule* GetInstance()
{
	return new NutrientCHRoute();
}

//! function to return the XML Metadata document string
extern "C" SEIMS_MODULE_API const char* MetadataInformation()
{
	MetadataInfo mdi;
	mdi.SetAuthor("Huiran Gao");
	mdi.SetClass(MCLS_NutCHRout, MCLSDESC_NutCHRout);
	mdi.SetDescription(MDESC_NutCHRout);
	mdi.SetEmail(SEIMS_EMAIL);
	mdi.SetID(MDESC_NutCHRout);
	mdi.SetName(MDESC_NutCHRout);
	mdi.SetVersion("1.0");
	mdi.SetWebsite(SEIMS_SITE);
	mdi.SetHelpfile("NutCHRout.html");

	// set the parameters
	mdi.AddParameter(VAR_RNUM1, UNIT_NON_DIM, DESC_RNUM1, Source_ParameterDB, DT_Single);
	mdi.AddParameter(VAR_IGROPT, UNIT_NON_DIM, DESC_IGROPT, Source_ParameterDB, DT_Single);
	mdi.AddParameter(VAR_AI0, UNIT_NUT_RATIO, DESC_AI0, Source_ParameterDB, DT_Single);
	mdi.AddParameter(VAR_AI1, UNIT_NUT_RATIO, DESC_AI1, Source_ParameterDB, DT_Single);
	mdi.AddParameter(VAR_AI2, UNIT_NUT_RATIO, DESC_AI2, Source_ParameterDB, DT_Single);
	mdi.AddParameter(VAR_AI3, UNIT_NUT_RATIO, DESC_AI3, Source_ParameterDB, DT_Single);
	mdi.AddParameter(VAR_AI4, UNIT_NUT_RATIO, DESC_AI4, Source_ParameterDB, DT_Single);
	mdi.AddParameter(VAR_AI5, UNIT_NUT_RATIO, DESC_AI5, Source_ParameterDB, DT_Single);
	mdi.AddParameter(VAR_AI6, UNIT_NUT_RATIO, DESC_AI6, Source_ParameterDB, DT_Single);
	mdi.AddParameter(VAR_LAMBDA0, UNIT_NON_DIM, DESC_LAMBDA0, Source_ParameterDB, DT_Single);
	mdi.AddParameter(VAR_LAMBDA1, UNIT_NON_DIM, DESC_LAMBDA1, Source_ParameterDB, DT_Single);
	mdi.AddParameter(VAR_LAMBDA2, UNIT_NON_DIM, DESC_LAMBDA2, Source_ParameterDB, DT_Single);
	mdi.AddParameter(VAR_K_L, UNIT_SR, DESC_K_L, Source_ParameterDB, DT_Single);
	mdi.AddParameter(VAR_K_N, UNIT_DENSITY_L, DESC_K_N, Source_ParameterDB, DT_Single);
	mdi.AddParameter(VAR_K_P, UNIT_DENSITY_L, DESC_K_P, Source_ParameterDB, DT_Single);
	mdi.AddParameter(VAR_P_N, UNIT_DENSITY_L, DESC_P_N, Source_ParameterDB, DT_Array1D);
	mdi.AddParameter(VAR_TFACT, UNIT_NON_DIM, DESC_TFACT, Source_ParameterDB, DT_Single);
	mdi.AddParameter(VAR_MUMAX, UNIT_TEMP_DEG, DESC_MUMAX, Source_ParameterDB, DT_Single);
	mdi.AddParameter(VAR_RHOQ, UNIT_PER_DAY, DESC_RHOQ, Source_ParameterDB, DT_Single);
	//mdi.AddParameter(VAR_MSK_X, UNIT_NON_DIM, DESC_MSK_X, Source_ParameterDB, DT_Single);
	//mdi.AddParameter(VAR_MSK_CO1, UNIT_NON_DIM, DESC_MSK_CO1, Source_ParameterDB, DT_Single);
	//mdi.AddParameter(VAR_VSF, UNIT_NON_DIM, DESC_VSF, Source_ParameterDB, DT_Single);

	mdi.AddParameter(VAR_DAYL, UNIT_TIMESTEP_HOUR, DESC_DAYL, Source_ParameterDB, DT_Array1D);
	mdi.AddParameter(VAR_SRA, UNIT_SR, DESC_SRA, Source_ParameterDB, DT_Array1D);
	mdi.AddParameter(VAR_WATTEMP, UNIT_TEMP_DEG, DESC_WATTEMP, Source_ParameterDB, DT_Array1D);

	// set the input variables
	mdi.AddInput(VAR_LATNO3_CH, UNIT_KG, DESC_LATNO3_CH, Source_Module, DT_Array1D);
	mdi.AddInput(VAR_SURQNO3_CH, UNIT_KG, DESC_SURQNO3_CH, Source_Module, DT_Array1D);
	mdi.AddInput(VAR_SURQSOLP_CH, UNIT_KG, DESC_SURQSOLP_CH, Source_Module, DT_Array1D);
	mdi.AddInput(VAR_NO3GW_CH, UNIT_KG, DESC_NO3GW_CH, Source_Module, DT_Array1D);
	mdi.AddInput(VAR_MINPGW_CH, UNIT_KG, DESC_MINPGW_CH, Source_Module, DT_Array1D);
	mdi.AddInput(VAR_SEDORGN_CH, UNIT_KG, DESC_SEDORGN_CH, Source_Module, DT_Array1D);
	mdi.AddInput(VAR_SEDORGP_CH, UNIT_KG, DESC_SEDORGP_CH, Source_Module, DT_Array1D);
	mdi.AddInput(VAR_SEDMINPA_CH, UNIT_KG, DESC_SEDMINPA_CH, Source_Module, DT_Array1D);
	mdi.AddInput(VAR_SEDMINPS_CH, UNIT_KG, DESC_SEDMINPS_CH, Source_Module, DT_Array1D);
	mdi.AddInput(VAR_AMMO_CH, UNIT_KG, DESC_AMMO_CH, Source_Module, DT_Array1D);
	mdi.AddInput(VAR_NITRITE_CH, UNIT_KG, DESC_NITRITE_CH, Source_Module, DT_Array1D);

	// set the output variables
	mdi.AddOutput(VAR_SOXY, UNIT_DENSITY_L, DESC_SOXY, DT_Array1D);
	mdi.AddOutput(VAR_ALGAE, UNIT_DENSITY_L, DESC_ALGAE, DT_Array1D);
	mdi.AddOutput(VAR_ORGANICN, UNIT_DENSITY_L, DESC_ORGANICN, DT_Array1D);
	mdi.AddOutput(VAR_ORGANICP, UNIT_DENSITY_L, DESC_ORGANICP, DT_Array1D);
	mdi.AddOutput(VAR_AMMONIAN, UNIT_DENSITY_L, DESC_AMMONIAN, DT_Array1D);
	mdi.AddOutput(VAR_NITRITEN, UNIT_DENSITY_L, DESC_NITRITEN, DT_Array1D);
	mdi.AddOutput(VAR_NITRATEN, UNIT_DENSITY_L, DESC_NITRATEN, DT_Array1D);
	mdi.AddOutput(VAR_DISOLVP, UNIT_DENSITY_L, DESC_DISOLVP, DT_Array1D);
	mdi.AddOutput(VAR_RCH_CBOD, UNIT_DENSITY_L, DESC_RCH_CBOD, DT_Array1D);
	mdi.AddOutput(VAR_RCH_DOX, UNIT_DENSITY_L, DESC_RCH_DOX, DT_Array1D);
	mdi.AddOutput(VAR_CHLORA, UNIT_DENSITY_L, DESC_CHLORA, DT_Array1D);

	string res = mdi.GetXMLDocument();
	char* tmp = new char[res.size()+1];
	strprintf(tmp, res.size()+1, "%s", res.c_str());
	return tmp;
}