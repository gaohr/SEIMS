#include "SEDR_SBAGNOLD.h"
#include "MetadataInfo.h"
#include "ModelException.h"
#include "util.h"
#include <omp.h>
#include <cmath>
#include <iostream>
#include <set>
#include <sstream>
#include <algorithm> 
#include <omp.h>

using namespace std;

SEDR_SBAGNOLD::SEDR_SBAGNOLD(void) : m_dt(-1), m_nreach(-1), m_sedtoCh(NULL), m_Chs0(NODATA_VALUE), m_sedChi0(NODATA_VALUE),
                           m_ptSub(NULL), m_chStorage(NULL),m_preChStorage(NULL), m_sedOut(NULL), m_VCD(-1),
                           m_reachDownStream(NULL), m_chOrder(NULL), m_chWidth(NULL), 
						   m_chLen(NULL), m_chDepth(NULL), m_chVel(NULL), m_chCover(NULL), m_chErod(NULL), m_qchOut(NULL),
                           m_prf(NODATA_VALUE), m_spcon(NODATA_VALUE), m_spexp(NODATA_VALUE), m_vcrit(NODATA_VALUE),
						   m_sedStorage(NULL), m_sedDep(NULL), m_sedDeg(NULL), m_sedConc(NULL),
						   m_rchSand(NULL),m_rchSilt(NULL),m_rchClay(NULL),m_rchSag(NULL),m_rchLag(NULL),m_rchGra(NULL),
						   m_rchBankEro(NULL),m_rchDeg(NULL),m_rchDep(NULL),m_flplainDep(NULL)
{
}

SEDR_SBAGNOLD::~SEDR_SBAGNOLD(void)
{
	if (m_reachDownStream != NULL) Release1DArray(m_reachDownStream);
	if (m_chOrder != NULL) Release1DArray(m_chOrder);
	if (m_chWidth != NULL) Release1DArray(m_chWidth);
	if (m_chLen != NULL) Release1DArray(m_chLen);
	if (m_chDepth != NULL) Release1DArray(m_chDepth);
	if (m_chVel != NULL) Release1DArray(m_chVel);
	if (m_chCover != NULL) Release1DArray(m_chCover);
	if (m_chErod != NULL) Release1DArray(m_chErod);
	if (m_ptSub != NULL) Release1DArray(m_ptSub);
    if (m_sedOut != NULL) Release1DArray(m_sedOut);
	if (m_sedConc != NULL) Release1DArray(m_sedConc);
    if (m_sedStorage != NULL) Release1DArray(m_sedStorage);
    if (m_sedDeg != NULL) Release1DArray(m_sedDeg);
    if (m_sedDep != NULL) Release1DArray(m_sedDep);


	if (m_rchSand != NULL) Release1DArray(m_rchSand);
	if (m_rchSilt != NULL) Release1DArray(m_rchSilt);
	if (m_rchClay != NULL) Release1DArray(m_rchClay);
	if (m_rchSag != NULL) Release1DArray(m_rchSag);
	if (m_rchLag != NULL) Release1DArray(m_rchLag);
	if (m_rchGra != NULL) Release1DArray(m_rchGra);
	if (m_rchBankEro != NULL) Release1DArray(m_rchBankEro);
	if (m_rchDeg != NULL) Release1DArray(m_rchDeg);
	if (m_rchDep != NULL) Release1DArray(m_rchDep);
	if (m_flplainDep != NULL) Release1DArray(m_flplainDep);
}

bool SEDR_SBAGNOLD::CheckInputData(void)
{
    if (m_dt < 0)
        throw ModelException(MID_SEDR_SBAGNOLD, "CheckInputData", "The parameter: m_dt has not been set.");

    if (m_nreach < 0)
        throw ModelException(MID_SEDR_SBAGNOLD, "CheckInputData", "The parameter: m_nreach has not been set.");

    if (FloatEqual(m_Chs0, NODATA_VALUE))
        throw ModelException(MID_SEDR_SBAGNOLD, "CheckInputData", "The parameter: Chs0 has not been set.");

    if (FloatEqual(m_prf, NODATA_VALUE))
        throw ModelException(MID_SEDR_SBAGNOLD, "CheckInputData", "The parameter: m_prf has not been set.");

    if (FloatEqual(m_spcon, NODATA_VALUE))
        throw ModelException(MID_SEDR_SBAGNOLD, "CheckInputData", "The parameter: m_spcon has not been set.");
    
    if (FloatEqual(m_spexp, NODATA_VALUE))
        throw ModelException(MID_SEDR_SBAGNOLD, "CheckInputData", "The parameter: m_spexp has not been set.");
    
    if (FloatEqual(m_vcrit, NODATA_VALUE))
        throw ModelException(MID_SEDR_SBAGNOLD, "CheckInputData", "The parameter: m_vcrit has not been set.");
    
	if (m_VCD < 0)
		throw ModelException(MID_SEDR_SBAGNOLD, "CheckInputData", "The parameter: m_VCD must be 0 or 1.");

    if (m_sedtoCh == NULL)
        throw ModelException(MID_SEDR_SBAGNOLD, "CheckInputData", "The parameter: m_sedtoCh has not been set.");
    
    if (m_chWidth == NULL)
        throw ModelException(MID_SEDR_SBAGNOLD, "CheckInputData", "The parameter: ReachParameter has not been set.");
    
    if (m_chStorage == NULL)
        throw ModelException(MID_SEDR_SBAGNOLD, "CheckInputData", "The parameter: m_chStorage has not been set.");
    
    if (m_chWTdepth == NULL)
        throw ModelException(MID_SEDR_SBAGNOLD, "CheckInputData", "The parameter: m_chWTdepth has not been set.");
    
    if (m_qchOut == NULL)
        throw ModelException(MID_SEDR_SBAGNOLD, "CheckInputData", "The parameter: m_qchOut has not been set.");
    
    return true;
}

void  SEDR_SBAGNOLD::initialOutputs()
{
    if (m_nreach <= 0)
        throw ModelException(MID_SEDR_SBAGNOLD, "initialOutputs", "The cell number of the input can not be less than zero.");

    if (m_reachLayers.empty())
    {
        CheckInputData();
        for (int i = 1; i <= m_nreach; i++)
        {
            int order = (int) m_chOrder[i];
            m_reachLayers[order].push_back(i);
        }
    }

    //initial channel storage
    if (m_sedOut == NULL)
    {       
		Initialize1DArray(m_nreach+1, m_sedOut, 0.f);
		Initialize1DArray(m_nreach+1, m_sedConc, 0.f);
		Initialize1DArray(m_nreach+1, m_sedStorage, 0.f);
		Initialize1DArray(m_nreach+1, m_sedDep, 0.f);
		Initialize1DArray(m_nreach+1, m_sedDeg, 0.f);

        for (int i = 1; i <= m_nreach; i++){
            //m_sedStorage[i] = m_Chs0 * m_chLen[i]; // m_Chs0 is initial channel storage per meter, not sediment! By LJ
			//m_sedStorage[i] = m_sedChi0 * m_Chs0 * m_chLen[i] * 1000.f;
			m_sedStorage[i] = m_sedChi0 * m_chStorage[i] * 1000.f; /// ton/m3 * m3/m * m * 1000 = kg
		}
		Initialize1DArray(m_nreach+1, m_rchSand, 0.f);
		Initialize1DArray(m_nreach+1, m_rchSilt, 0.f);
		Initialize1DArray(m_nreach+1, m_rchClay, 0.f);
		Initialize1DArray(m_nreach+1, m_rchSag, 0.f);
		Initialize1DArray(m_nreach+1, m_rchLag, 0.f);
		Initialize1DArray(m_nreach+1, m_rchGra, 0.f);
		Initialize1DArray(m_nreach+1, m_rchBankEro, 0.f);
		Initialize1DArray(m_nreach+1, m_rchDeg, 0.f);
		Initialize1DArray(m_nreach+1, m_rchDep, 0.f);
		Initialize1DArray(m_nreach+1, m_flplainDep, 0.f);
    }
	/// initialize point source loadings
	if (m_ptSub == NULL) Initialize1DArray(m_nreach+1, m_ptSub, 0.f);
}

void SEDR_SBAGNOLD::PointSourceLoading()
{
	/// load point source water discharge (m3/s) on current day from Scenario
	for (map<int, BMPPointSrcFactory*>::iterator it = m_ptSrcFactory.begin(); it != m_ptSrcFactory.end(); it++)
	{
		//cout<<"unique Point Source Factory ID: "<<it->first<<endl;
		vector<int> m_ptSrcMgtSeqs = it->second->GetPointSrcMgtSeqs();
		map<int, PointSourceMgtParams *>  m_pointSrcMgtMap = it->second->GetPointSrcMgtMap();
		vector<int> m_ptSrcIDs = it->second->GetPointSrcIDs();
		map<int, PointSourceLocations *> m_pointSrcLocsMap = it->second->GetPointSrcLocsMap();
		// 1. looking for management operations from m_pointSrcMgtMap
		for (vector<int>::iterator seqIter = m_ptSrcMgtSeqs.begin(); seqIter != m_ptSrcMgtSeqs.end(); seqIter++)
		{
			PointSourceMgtParams* curPtMgt = m_pointSrcMgtMap.at(*seqIter);
			// 1.1 If current day is beyond the date range, then continue to next management
			if(curPtMgt->GetStartDate() != 0 && curPtMgt->GetEndDate() != 0)
			{
				if (m_date < curPtMgt->GetStartDate() || m_date > curPtMgt->GetEndDate())
					continue;
			}
			// 1.2 Otherwise, get the water volume
			float per_sed = curPtMgt->GetSedment(); /// g/cm3, or Mg/m3 
			// 1.3 Sum up all point sources
			for (vector<int>::iterator locIter = m_ptSrcIDs.begin(); locIter != m_ptSrcIDs.end(); locIter++)
			{
				if (m_pointSrcLocsMap.find(*locIter) != m_pointSrcLocsMap.end()){
					PointSourceLocations* curPtLoc = m_pointSrcLocsMap.at(*locIter);
					int curSubID = curPtLoc->GetSubbasinID();
					/// Mg/m3 ==> kg / timestep
					m_ptSub[curSubID] += per_sed * curPtLoc->GetSize() * 1000.f * m_dt / 86400.f; 
				}
			}
		}
	}
}

int SEDR_SBAGNOLD::Execute()
{
    //check the data
    CheckInputData();
    initialOutputs();
	/// load point source water volume from m_ptSrcFactory
	PointSourceLoading();
    map<int, vector<int> >::iterator it;
    for (it = m_reachLayers.begin(); it != m_reachLayers.end(); it++)
    {
        // There are not any flow relationship within each routing layer.
        // So parallelization can be done here.
        int nReaches = it->second.size();
        // the size of m_reachLayers (map) is equal to the maximum stream order
#pragma omp parallel for
        for (int i = 0; i < nReaches; ++i)
        {
            int reachIndex = it->second[i]; // index in the array
            SedChannelRouting(reachIndex);
			// compute changes in channel dimensions caused by downcutting and widening
			if(m_VCD)
				doChannelDowncuttingAndWidening(reachIndex);
        }
    }
    return 0;
}

bool SEDR_SBAGNOLD::CheckInputSize(const char *key, int n)
{
    if (n <= 0)
    {
        //this->StatusMsg("Input data for "+string(key) +" is invalid. The size could not be less than zero.");
        return false;
    }
    if (this->m_nreach != n)
    {
        if (this->m_nreach <= 0) this->m_nreach = n;
        else
        {
            //this->StatusMsg("Input data for "+string(key) +" is invalid. All the input data should have same size.");
            ostringstream oss;
            oss << "Input data for " + string(key) << " is invalid with size: " << n << ". The origin size is " <<
            m_nreach << ".\n";
            throw ModelException(MID_SEDR_SBAGNOLD, "CheckInputSize", oss.str());
        }
    }
    return true;
}

void SEDR_SBAGNOLD::GetValue(const char *key, float *value)
{
    string sk(key);
    int iOutlet = m_reachLayers.rbegin()->second[0];
    if (StringMatch(sk, VAR_SED_OUTLET))*value = m_sedOut[iOutlet];
	else
		throw ModelException(MID_SEDR_SBAGNOLD, "GetValue", "Parameter " + sk + " does not exist.");
}

void SEDR_SBAGNOLD::SetValue(const char *key, float value)
{
    string sk(key);

    if (StringMatch(sk, VAR_OMP_THREADNUM))
    {
        omp_set_num_threads((int) value);
    }
#ifdef STORM_MODEL
        else if (StringMatch(sk, Tag_ChannelTimeStep))
        {
            m_dt = (int) value;
        }
#else
    else if (StringMatch(sk, Tag_TimeStep))
    {
        m_dt = (int) value;
    }
#endif
    else if (StringMatch(sk, VAR_P_RF))
    {
        m_prf = value;
    }
    else if (StringMatch(sk, VAR_SPCON))
    {
        m_spcon = value;
    }
    else if (StringMatch(sk, VAR_SPEXP))
    {
        m_spexp = value;
    }
    else if (StringMatch(sk, VAR_VCRIT))
    {
        m_vcrit = value;
    }
    else if (StringMatch(sk, VAR_CHS0))
    {
        m_Chs0 = value;
    }
	else if (StringMatch(sk, VAR_SED_CHI0)) m_sedChi0 = value;
	else if (StringMatch(sk, VAR_VCD)) m_VCD = (int)value;
    else
        throw ModelException(MID_SEDR_SBAGNOLD, "SetValue", "Parameter " + sk + " does not exist.");
}

void SEDR_SBAGNOLD::Set1DData(const char *key, int n, float *data)
{
    string sk(key);
    //check the input data
    if (StringMatch(sk, VAR_SED_TO_CH))
    {
        m_sedtoCh = data;   //for longterm model
    }
    else if (StringMatch(sk, VAR_SUB_SEDTOCH))
    {
        m_sedtoCh = data;   //for storm model
    }
    else if (StringMatch(sk, VAR_QRECH))
    {
        m_qchOut = data;
    }
    else if (StringMatch(sk, VAR_CHST))
    {
        m_chStorage = data;
    }
	else if (StringMatch(sk, VAR_PRECHST))
		m_preChStorage = data;
    else if (StringMatch(sk, VAR_CHWTDEPTH))
    {
        m_chWTdepth = data;
    }
	else if (StringMatch(sk, VAR_CHWTWIDTH))
		m_chWTWidth = data;
	else if (StringMatch(sk, VAR_PRECHWTDEPTH))
		m_preChWTDepth = data;
    else
        throw ModelException(MID_SEDR_SBAGNOLD, "Set1DData", "Parameter " + sk + " does not exist");
}

void SEDR_SBAGNOLD::Get1DData(const char *key, int *n, float **data)
{
	initialOutputs();
    string sk(key);
    *n = m_nreach + 1;
    int iOutlet = m_reachLayers.rbegin()->second[0];
    if (StringMatch(sk, VAR_SED_RECH))
    {
        m_sedOut[0] = m_sedOut[iOutlet];    // kg
        *data = m_sedOut;
    }
	else if (StringMatch(sk, VAR_SED_RECHConc))
	{
		m_sedConc[0] = m_sedConc[iOutlet];    // kg/m3, i.e., g/L
		*data = m_sedConc;
	}
	else if (StringMatch(sk, VAR_RCH_DEG))
		*data = m_rchDeg;
	else if (StringMatch(sk, VAR_RCH_BANKERO))
		*data = m_rchBankEro;
	else if (StringMatch(sk, VAR_RCH_DEP))
		*data = m_rchDep;
	else if (StringMatch(sk, VAR_FLPLAIN_DEP))
		*data = m_flplainDep;
    else
        throw ModelException(MID_SEDR_SBAGNOLD, "Get1DData", "Output " + sk + " does not exist.");
}

void SEDR_SBAGNOLD::Get2DData(const char *key, int *nRows, int *nCols, float ***data)
{
    string sk(key);
}

void SEDR_SBAGNOLD::SetScenario(Scenario *sce)
{
	if (sce != NULL){
		map <int, BMPFactory* > *tmpBMPFactories = sce->GetBMPFactories();
		for (map<int, BMPFactory *>::iterator it = tmpBMPFactories->begin(); it != tmpBMPFactories->end(); it++)
		{
			/// Key is uniqueBMPID, which is calculated by BMP_ID * 100000 + subScenario;
			if (it->first / 100000 == BMP_TYPE_POINTSOURCE)
			{
				m_ptSrcFactory[it->first] = (BMPPointSrcFactory*)it->second;
			}
		}
	}
	else
		throw ModelException(MID_SEDR_SBAGNOLD, "SetScenario", "The scenario can not to be NULL.");
}

void SEDR_SBAGNOLD::SetReaches(clsReaches *reaches)
{
	if(reaches != NULL)
	{
		m_nreach = reaches->GetReachNumber();
		m_reachId = reaches->GetReachIDs();
		if (m_reachDownStream == NULL)
		{
			Initialize1DArray(m_nreach+1,m_reachDownStream, 0.f);
			Initialize1DArray(m_nreach+1,m_chOrder, 0.f);
			Initialize1DArray(m_nreach+1,m_chWidth, 0.f);
			Initialize1DArray(m_nreach+1,m_chLen, 0.f);
			Initialize1DArray(m_nreach+1,m_chDepth, 0.f);
			Initialize1DArray(m_nreach+1,m_chVel, 0.f);
			Initialize1DArray(m_nreach+1,m_chSlope, 0.f);
			Initialize1DArray(m_nreach+1,m_chCover, 0.f);
			Initialize1DArray(m_nreach+1,m_chErod, 0.f);
		}
		for (vector<int>::iterator it = m_reachId.begin(); it != m_reachId.end(); it++)
		{
			int i = *it;
			clsReach* tmpReach = reaches->GetReachByID(i);
			m_reachDownStream[i] = (float)tmpReach->GetDownStream();
			m_chOrder[i] = (float)tmpReach->GetUpDownOrder();
			m_chWidth[i] = (float)tmpReach->GetWidth();
			m_chLen[i] = (float)tmpReach->GetLength();
			m_chDepth[i] = (float)tmpReach->GetDepth();
			m_chVel[i] = (float)tmpReach->GetV0();
			m_chCover[i] = (float)tmpReach->GetCover();
			m_chErod[i] = (float)tmpReach->GetErod();
		}
		m_reachUpStream.resize(m_nreach + 1);
		if (m_nreach > 1)
		{
			for (int i = 1; i <= m_nreach; i++)// index of the reach is the equal to streamlink ID(1 to nReaches)
			{
				int downStreamId = int(m_reachDownStream[i]);
				if (downStreamId <= 0)
					continue;
				m_reachUpStream[downStreamId].push_back(i);
			}
		}
	}
	else
		throw ModelException(MID_SEDR_SBAGNOLD, "SetReaches", "The reaches input can not to be NULL.");
}

void SEDR_SBAGNOLD::SedChannelRouting(int i)
{
	float qOutV = 0.f; // water volume (m^3) of flow out
	float allWater = 0.f; // // water in reach during time step, qdin in SWAT
	// initialize sediment in reach during time step
	/// sediment from upstream reaches
	float allSediment = 0.f; // all sediment in reach, kg
	float sedUp = 0.f; // sediment from upstream channels, kg
	for (size_t j = 0; j < m_reachUpStream[i].size(); ++j)
	{
		int upReachId = m_reachUpStream[i][j];
		sedUp += m_sedOut[upReachId];
	}
	allSediment = sedUp + m_sedtoCh[i] + m_sedStorage[i];
	/// add point source loadings
	if (m_ptSub != NULL && m_ptSub[i] > 0.f)
		allSediment += m_ptSub[i];
	//if (i == 12) cout<<"\tall sed: "<<allSediment<<", sed up: "<<sedUp<<", sed to ch: "<<m_sedtoCh[i]<<", strg: "<<m_sedStorage[i]<<endl;

	// initialize water in reach during time step
	qOutV = m_qchOut[i] * m_dt; // m^3
	//allWater = m_chStorage[i] + qOutV;
	allWater = m_preChStorage[i];
	if ((m_qchOut[i] < UTIL_ZERO && m_chWTdepth[i] < UTIL_ZERO) || allWater < 0.01f)
	{
		/// do not perform sediment routing when:
		/// 1. whether is no water flow out of channel and water depth is nearly zero
		/// 2. if no water in reach
		m_sedDeg[i] = 0.f;
		m_sedDep[i] = 0.f;
		m_sedOut[i] = 0.f;
		m_sedConc[i] = 0.f;
		m_rchSand[i] = 0.f;
		m_rchSilt[i] = 0.f;
		m_rchClay[i] = 0.f;
		m_rchSag[i] = 0.f;
		m_rchLag[i] = 0.f;
		m_rchGra[i] = 0.f;
		m_rchBankEro[i] = 0.f;
		m_rchDeg[i] = 0.f;
		m_rchDep[i] = 0.f;
		m_flplainDep[i] = 0.f;
		m_sedStorage[i] = allSediment;
		return;
	}
    // initialize reach peak runoff rate and calculate flow velocity
    float peakFlowRate = m_qchOut[i] * m_prf;
    float crossarea = allWater / m_chLen[i]; // SWAT, eq. 7:1.1.7
    float peakVelocity = 0.f;
	if (m_preChWTDepth[i] < 0.01f)
		peakVelocity = 0.01f;
	else
		peakVelocity = peakFlowRate / crossarea;
    if (peakVelocity > 5.f) peakVelocity = 5.f;

	/// calculate tbase  |none  |flow duration (fraction of 24 hr)
	float tbase = m_chLen[i] / (m_dt * peakVelocity);
	if (tbase > 1.f) tbase = 1.f;

	/// New imporoved method for sediment transport 
	float initCon = 0.f; // cyin
	float maxCon = 0.f; // cych
	float sedDeposition = 0.f; // depnet, and dep
	float sedDegradation = 0.f; // deg
	float sedDegradation1 = 0.f; // deg1
	float sedDegradation2 = 0.f; // deg2

	//deposition and degradation
    initCon = allSediment / allWater; // kg/m^3
    //max concentration
    maxCon = m_spcon * pow(peakVelocity, m_spexp) * 1000.f; // kg/m^3  
	//if (i == 12) cout<<"iniCon: "<<initCon<<", maxCon: "<<maxCon<<endl;
    //initial concentration,mix sufficiently
    sedDeposition = allWater * (initCon - maxCon); // kg
    if (peakVelocity < m_vcrit)
        sedDeposition = 0.f;
	
    if (sedDeposition < 0.f)    //degradation
    {
        sedDegradation = -sedDeposition * tbase;
		// first the deposited material will be degraded before channel bed
        if (sedDegradation >= m_sedDep[i])
        {
            sedDegradation1 = m_sedDep[i];
			sedDegradation2 = (sedDegradation - sedDegradation1) * m_chErod[i] * m_chCover[i];
        }
        else
        {
            sedDegradation1 = sedDegradation;
			sedDegradation2 = 0.f;
        }
        sedDeposition = 0.f;
    }
	else
	{
		sedDegradation = 0.f;
		sedDegradation1 = 0.f;
		sedDegradation2 = 0.f;
	}
    //update sed deposition
    m_sedDep[i] += sedDeposition - sedDegradation1;
	if (m_sedDep[i] < UTIL_ZERO) m_sedDep[i] = 0.f;
    m_sedDeg[i] += sedDegradation1 + sedDegradation2;

    //get sediment after deposition and degradation
    allSediment += sedDegradation1 + sedDegradation2 - sedDeposition;
	if (allSediment < UTIL_ZERO) allSediment = 0.f;
    //get out flow water fraction
    float outFraction = qOutV / allWater;
	if (outFraction > 1.f) outFraction = 1.f;
    m_sedOut[i] = allSediment * outFraction;
	if (m_sedOut[i] < UTIL_ZERO) m_sedOut[i] = 0.f;
    // update sediment storage
    m_sedStorage[i] = allSediment - m_sedOut[i];
	if (m_sedStorage[i] < UTIL_ZERO) m_sedStorage[i] = 0.f;

    // get final sediment in water, cannot large than 0.848 ton/m3
    float maxSedinWt = 0.848f * qOutV * 1000.f; /// kg
    if (m_sedOut[i] > maxSedinWt)
    {
        m_sedDep[i] += m_sedOut[i] - maxSedinWt;
        m_sedOut[i] = maxSedinWt;
    }
	/// calculate sediment concentration
	m_sedConc[i] = m_sedOut[i] / qOutV; /// kg/m3, i.e., g/L
	/// in this default sediment routing method, sediment is not tracked by particle size
	m_rchSand[i] = 0.f;
	m_rchSilt[i] = m_sedOut[i]; // the sediments is assumed to be silt for mass conservation
	m_rchClay[i] = 0.f;
	m_rchSag[i] = 0.f;
	m_rchLag[i] = 0.f;
	m_rchGra[i] = 0.f;

	m_rchBankEro[i] = 0.f;
	m_rchDeg[i] = sedDegradation2;
	m_rchDep[i] = sedDeposition;
	m_flplainDep[i] = 0.f;
	//if (i == 12) cout<<"\tallSediment2: "<<allSediment<<", sedDeg: "<<sedDegradation1<<", sedDeg2: "<<sedDegradation2<<
	//	", sedDeposition: "<<sedDeposition<<", sed flow out: "<<m_sedOut[i]<<endl;
}

void SEDR_SBAGNOLD::doChannelDowncuttingAndWidening(int id)
{
	/// TODO, lj
    float depdeg = m_preChWTDepth[id] - m_chWTdepth[id]; // depth of degradation/deposition from original
    if (depdeg < m_chSlope[id] * m_chLen[id])
    {
        //float storage = m_chStorage[id];
        //float vout = m_qchOut[id] * m_dt;
        if (m_preChStorage[id] > 1.4e6f)
        {
			/// downcutting depth, m
            float cutDepth = 358.6f * m_chWTdepth[id] * m_chSlope[id] * m_chErod[id];
            m_chWTdepth[id] += cutDepth;
            m_chWTWidth[id] = m_chWTdepth[id] * (m_chWidth[id] / m_chDepth[id]);

            m_chSlope[id] -= cutDepth / m_chLen[id];
            m_chSlope[id] = max(0.0001f, m_chSlope[id]);
        }
    }
	// call ttcoef(jrch) // TODO 
}
