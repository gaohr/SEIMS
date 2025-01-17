#include "MUSK_CH.h"
#include "MetadataInfo.h"
#include "ModelException.h"
#include "util.h"
#include <omp.h>

using namespace std;

//! Constructor
MUSK_CH::MUSK_CH(void) : m_dt(-1), m_nreach(-1), m_outletID(-1), m_Kchb(NODATA_VALUE),
                         m_Kbank(NODATA_VALUE), m_Epch(NODATA_VALUE), m_Bnk0(NODATA_VALUE), m_Chs0_perc(NODATA_VALUE), m_aBank(NODATA_VALUE),m_chSideSlope(NULL),
                         m_bBank(NODATA_VALUE), m_subbasin(NULL), m_qsSub(NULL),
						 m_reachDownStream(NULL), m_chOrder(NULL), m_chWidth(NULL), 
						 m_chLen(NULL), m_chDepth(NULL), m_chVel(NULL), m_area(NULL),
                         m_ptSub(NULL), m_qiSub(NULL), m_qgSub(NULL), m_petCh(NULL), m_gwStorage(NULL), m_Vseep0(0.f),
                         m_bankStorage(NULL), m_seepage(NULL),
                         m_qsCh(NULL), m_qiCh(NULL), m_qgCh(NULL),
                         m_x(NODATA_VALUE), m_co1(NODATA_VALUE), m_qIn(NULL), m_chStorage(NULL),m_preChStorage(NULL), m_vScalingFactor(1.0f),
                         m_qUpReach(0.f), m_deepGroundwater(0.f),m_chWTdepth(NULL), m_preChWTDepth(NULL), m_chWTWidth(NULL),m_chBtmWidth(NULL)
{
}

//! Destructor
MUSK_CH::~MUSK_CH(void)
{
	if (m_reachDownStream != NULL) Release1DArray(m_reachDownStream);
	if (m_chOrder != NULL) Release1DArray(m_chOrder);
	if (m_chWidth != NULL) Release1DArray(m_chWidth);
	if (m_chLen != NULL) Release1DArray(m_chLen);
	if (m_chDepth != NULL) Release1DArray(m_chDepth);
	if (m_chVel != NULL) Release1DArray(m_chVel);
    if (m_area != NULL) Release1DArray(m_area);
    if (m_chStorage != NULL) Release1DArray(m_chStorage);
	if (m_preChStorage != NULL) Release1DArray(m_preChStorage);
    if (m_qOut != NULL) Release1DArray(m_qOut);
    if (m_bankStorage != NULL)Release1DArray(m_bankStorage);
    if (m_seepage != NULL)Release1DArray(m_seepage);
    if (m_qsCh != NULL)Release1DArray(m_qsCh);
    if (m_qiCh != NULL)Release1DArray(m_qiCh);
    if (m_qgCh != NULL)Release1DArray(m_qgCh);
    if (m_chWTdepth != NULL)Release1DArray(m_chWTdepth);
	if (m_preChWTDepth != NULL)Release1DArray(m_preChWTDepth);
	if (m_chWTWidth != NULL) Release1DArray(m_chWTWidth);
	if (m_chBtmWidth != NULL) Release1DArray(m_chBtmWidth);
	if (m_ptSub != NULL) Release1DArray(m_ptSub);
	if (!m_ptSrcFactory.empty())
	{
		for (map<int, BMPPointSrcFactory*>::iterator it = m_ptSrcFactory.begin(); it != m_ptSrcFactory.end();)
		{
			if(it->second != NULL)
				delete it->second;
			it = m_ptSrcFactory.erase(it);
		}
		m_ptSrcFactory.clear();
	}
}

//! Check input data
bool MUSK_CH::CheckInputData(void)
{
    if (m_dt < 0)
        throw ModelException(MID_MUSK_CH, "CheckInputData", "The parameter: m_dt has not been set.");
    if (m_nreach < 0)
		throw ModelException(MID_MUSK_CH, "CheckInputData", "The parameter: m_nreach has not been set.");
	if (FloatEqual(m_x, NODATA_VALUE))
		throw ModelException(MID_MUSK_CH, "CheckInputData", "The parameter: m_x has not been set.");
	if (FloatEqual(m_co1, NODATA_VALUE))
		throw ModelException(MID_MUSK_CH, "CheckInputData", "The parameter: m_co1 has not been set.");
    if (FloatEqual(m_Kchb, NODATA_VALUE))
        throw ModelException(MID_MUSK_CH, "CheckInputData", "The parameter: K_chb has not been set.");
    if (FloatEqual(m_Kbank, NODATA_VALUE))
        throw ModelException(MID_MUSK_CH, "CheckInputData", "The parameter: K_bank has not been set.");
    if (FloatEqual(m_Epch, NODATA_VALUE))
        throw ModelException(MID_MUSK_CH, "CheckInputData", "The parameter: Ep_ch has not been set.");
    if (FloatEqual(m_Bnk0, NODATA_VALUE))
        throw ModelException(MID_MUSK_CH, "CheckInputData", "The parameter: Bnk0 has not been set.");
    if (FloatEqual(m_Chs0_perc, NODATA_VALUE))
        throw ModelException(MID_MUSK_CH, "CheckInputData", "The parameter: Chs0 has not been set.");
    if (FloatEqual(m_aBank, NODATA_VALUE))
        throw ModelException(MID_MUSK_CH, "CheckInputData", "The parameter: A_bnk has not been set.");
    if (FloatEqual(m_bBank, NODATA_VALUE))
        throw ModelException(MID_MUSK_CH, "CheckInputData", "The parameter: B_bnk has not been set.");
    if (FloatEqual(m_Vseep0, NODATA_VALUE))
        throw ModelException(MID_MUSK_CH, "CheckInputData", "The parameter: m_Vseep0 has not been set.");
    if (m_subbasin == NULL)
        throw ModelException(MID_MUSK_CH, "CheckInputData", "The parameter: m_subbasin has not been set.");
    if (m_qsSub == NULL)
        throw ModelException(MID_MUSK_CH, "CheckInputData", "The parameter: Q_SBOF has not been set.");
    //if (m_qiSub == NULL)
    //	throw ModelException(MID_MUSK_CH,"CheckInputData","The parameter: Q_SBIF has not been set.");
    //if (m_qgSub == NULL)
    //	throw ModelException(MID_MUSK_CH,"CheckInputData","The parameter: QG_sub has not been set.");
	//if (m_petCh == NULL)
	//	throw ModelException(MID_MUSK_CH,"CheckInputData","The parameter: SBPET has not been set.");
    //if (m_gwStorage == NULL)
    //{
    //	throw ModelException(MID_MUSK_CH,"CheckInputData","The parameter: GW_sub has not been set.");
    //}
    if (m_chWidth == NULL)
        throw ModelException(MID_MUSK_CH, "CheckInputData", "The parameter: RchParam has not been set.");
    return true;
}

//! Initial outputs
void  MUSK_CH::initialOutputs()
{
    if (m_nreach <= 0)
        throw ModelException(MID_MUSK_CH, "initialOutputs", "The reach number can not be less than zero.");

    if (m_reachLayers.empty())
    {
        for (int i = 1; i <= m_nreach; i++)
        {
			if (m_chOrder == NULL)
				throw ModelException(MID_MUSK_CH, "initialOutputs", "Stream order is not loaded successful from Reach table.");
            int order = (int) m_chOrder[i];
            m_reachLayers[order].push_back(i);
        }
    }
	if (m_outletID < 0)
		m_outletID = m_reachLayers.rbegin()->second[0];
    //initial channel storage
    if (m_chStorage == NULL)
    {
        m_chStorage = new float[m_nreach + 1];
		m_preChStorage = new float[m_nreach + 1];
        m_qIn = new float[m_nreach + 1];
        m_qOut = new float[m_nreach + 1];
        m_bankStorage = new float[m_nreach + 1];
        m_seepage = new float[m_nreach + 1];
        m_qsCh = new float[m_nreach + 1];
        m_qiCh = new float[m_nreach + 1];
        m_qgCh = new float[m_nreach + 1];
        m_chWTdepth = new float[m_nreach + 1];
		m_preChWTDepth = new float[m_nreach + 1];
		m_chWTWidth = new float[m_nreach + 1];
		m_chBtmWidth = new float[m_nreach + 1];

        for (int i = 1; i <= m_nreach; i++)
        {
            float qiSub = 0.f;
            float qgSub = 0.f;
            if (m_qiSub != NULL) // interflow (subsurface flow)
                qiSub = m_qiSub[i];
            if (m_qgSub != NULL) // groundwater
                qgSub = m_qgSub[i];
            m_seepage[i] = 0.f;
            m_bankStorage[i] = m_Bnk0 * m_chLen[i];
			m_chBtmWidth[i] = m_chWidth[i] - 2.f * m_chSideSlope[i] * m_chDepth[i];
			if (m_chBtmWidth[i] <= UTIL_ZERO){
				m_chBtmWidth[i] = 0.5f * m_chWidth[i];
				m_chSideSlope[i] = (m_chWidth[i] - m_chBtmWidth[i])/2.f/m_chDepth[i];
			}
			m_chWTdepth[i] = m_chDepth[i] * m_Chs0_perc;
			m_chWTWidth[i] = m_chBtmWidth[i]+ 2.f * m_chSideSlope[i] * m_chWTdepth[i];
			m_preChWTDepth[i] = m_chWTWidth[i];
			// m_chWTWidth[i] = m_chWidth[i];
			// m_chStorage[i] = m_chWTdepth[i] * m_chWTWidth[i] * m_chLen[i];
            m_chStorage[i] = m_chLen[i] * m_chWTdepth[i] * (m_chBtmWidth[i] + m_chSideSlope[i] * m_chWTdepth[i]);
			m_preChStorage[i] = m_chStorage[i];
			m_qIn[i] = 0.f;
            m_qOut[i] = m_qsSub[i] + qiSub + qgSub;
            m_qsCh[i] = m_qsSub[i];
            m_qiCh[i] = qiSub;
            m_qgCh[i] = qgSub;
        }
    }
	/// initialize point source loadings
	if (m_ptSub == NULL)
		Initialize1DArray(m_nreach+1,m_ptSub,0.f);
}
void MUSK_CH::PointSourceLoading()
{
	/// load point source water discharge (m3/s) on current day from Scenario
	for (map<int, BMPPointSrcFactory*>::iterator it = m_ptSrcFactory.begin(); it != m_ptSrcFactory.end(); it++)
	{
		/// reset point source loading water to 0.f
		for(int i = 0; i <= m_nreach; i++)
			m_ptSub[i] = 0.f;
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
			float per_wtrVol = curPtMgt->GetWaterVolume(); /// m3/'size'/day 
			// 1.3 Sum up all point sources
			for (vector<int>::iterator locIter = m_ptSrcIDs.begin(); locIter != m_ptSrcIDs.end(); locIter++)
			{
				if (m_pointSrcLocsMap.find(*locIter) != m_pointSrcLocsMap.end()){
					PointSourceLocations* curPtLoc = m_pointSrcLocsMap.at(*locIter);
					int curSubID = curPtLoc->GetSubbasinID();
					m_ptSub[curSubID] += per_wtrVol * curPtLoc->GetSize() / 86400.f; /// m3/'size'/day ==> m3/s
				}
			}
		}
	}
}
//! Execute function
int MUSK_CH::Execute()
{
	CheckInputData();
    initialOutputs();
	/// load point source water volume from m_ptSrcFactory
	PointSourceLoading();
    map<int, vector<int> >::iterator it;
    for (it = m_reachLayers.begin(); it != m_reachLayers.end(); it++)
    {
        // There are not any flow relationship within each routing layer.
        // So parallelization can be done here.
        int reachNum = it->second.size();
        // the size of m_reachLayers (map) is equal to the maximum stream order
#pragma omp parallel for
        for (int i = 0; i < reachNum; ++i)
        {
            int reachIndex = it->second[i]; // index in the array
            ChannelFlow(reachIndex);
        }
    }
    return 0;
}

//! Check input size
bool MUSK_CH::CheckInputSize(const char *key, int n)
{
    if (n <= 0)
		throw ModelException(MID_MUSK_CH, "CheckInputSize",
		"Input data for " + string(key) + " is invalid. The size could not be less than zero.");
#ifdef STORM_MODEL
    if(m_nreach != n-1)
    {
        if(m_nreach <=0)
            m_nreach = n-1;
        else
        {
            //StatusMsg("Input data for "+string(key) +" is invalid. All the input data should have same size.");
            ostringstream oss;
            oss << "Input data for "+string(key) << " is invalid with size: " << n << ". The origin size is " << m_nreach << ".\n";
            throw ModelException(MID_MUSK_CH,"CheckInputSize",oss.str());
        }
    }
#else
    if (m_nreach != n - 1)
    {
        if (m_nreach <= 0)
            m_nreach = n - 1;
        else
        {
            //StatusMsg("Input data for "+string(key) +" is invalid. All the input data should have same size.");
            ostringstream oss;
            oss << "Input data for " + string(key) << " is invalid with size: " << n << ". The origin size is " <<
            m_nreach << ".\n";
            throw ModelException(MID_MUSK_CH, "CheckInputSize", oss.str());
        }
    }
#endif
    return true;
}

//! Set value of the module
void MUSK_CH::SetValue(const char *key, float value)
{
    string sk(key);

    if (StringMatch(sk, VAR_QUPREACH)) m_qUpReach = value;
    else if (StringMatch(sk, VAR_VSF)) m_vScalingFactor = value;
    else if (StringMatch(sk, Tag_ChannelTimeStep)) m_dt = (int) value;
    else if (StringMatch(sk, VAR_OMP_THREADNUM))omp_set_num_threads((int) value);
    else if (StringMatch(sk, VAR_K_CHB))m_Kchb = value;
    else if (StringMatch(sk, VAR_K_BANK))m_Kbank = value;
    else if (StringMatch(sk, VAR_EP_CH))m_Epch = value;
    else if (StringMatch(sk, VAR_BNK0))m_Bnk0 = value;
    else if (StringMatch(sk, VAR_CHS0_PERC))m_Chs0_perc = value;
    else if (StringMatch(sk, VAR_VSEEP0))m_Vseep0 = value;
    else if (StringMatch(sk, VAR_A_BNK))m_aBank = value;
    else if (StringMatch(sk, VAR_B_BNK))m_bBank = value;
    else if (StringMatch(sk, VAR_MSK_X))m_x = value;
	else if (StringMatch(sk, VAR_MSK_CO1))m_co1 = value;
	else if (StringMatch(sk, VAR_GWRQ))m_deepGroundwater = value;
    else
        throw ModelException(MID_MUSK_CH, "SetValue", "Parameter " + sk + " does not exist.");
}

//! Set 1D data
void MUSK_CH::Set1DData(const char *key, int n, float *value)
{
    string sk(key);
    //check the input data
	
    if (StringMatch(sk, VAR_SUBBSN))m_subbasin = value;
    else if (StringMatch(sk, VAR_SBOF))
    {
        CheckInputSize(key, n);
        m_qsSub = value;
    }
    else if (StringMatch(sk, VAR_SBIF))
    {
        CheckInputSize(key, n);
        m_qiSub = value;
    }
    else if (StringMatch(sk, VAR_SBQG))
    {
        m_qgSub = value;
    }
    else if (StringMatch(sk, VAR_SBPET))
    {
        m_petCh = value;
    }
    else if (StringMatch(sk, VAR_SBGS))
    {
        m_gwStorage = value;
    }
    else
        throw ModelException(MID_MUSK_CH, "Set1DData", "Parameter " + sk + " does not exist.");
}

//! Get value
void MUSK_CH::GetValue(const char *key, float *value)
{
    string sk(key);
    int iOutlet = m_reachLayers.rbegin()->second[0];
    if (StringMatch(sk, VAR_QOUTLET))
    {
        m_qOut[0] = m_qOut[iOutlet];
        *value = m_qOut[0];
    }
    else if (StringMatch(sk, VAR_QSOUTLET))
    {
        *value = m_qsCh[iOutlet];
    }
}

//! Get 1D data
void MUSK_CH::Get1DData(const char *key, int *n, float **data)
{
	if(m_reachLayers.empty())
		initialOutputs();
    string sk(key);
    *n = m_nreach + 1;
    //int m_outletID = m_reachLayers.rbegin()->second[0];
    if (StringMatch(sk, VAR_QRECH))
    {
        m_qOut[0] = m_qOut[m_outletID];
        *data = m_qOut;
    }
    else if (StringMatch(sk, VAR_QS))
    {
        m_qsCh[0] = m_qsCh[m_outletID];
        *data = m_qsCh;
    }
    else if (StringMatch(sk, VAR_QI))
    {
        m_qiCh[0] = m_qiCh[m_outletID];
        *data = m_qiCh;
    }
    else if (StringMatch(sk, VAR_QG))
    {
        m_qgCh[0] = m_qgCh[m_outletID];
        *data = m_qgCh;
    }
    else if (StringMatch(sk, VAR_BKST))
    {
        m_bankStorage[0] = m_bankStorage[m_outletID];
        *data = m_bankStorage;
    }
    else if (StringMatch(sk, VAR_CHST))
    {
        m_chStorage[0] = m_chStorage[m_outletID];
        *data = m_chStorage;
    }
	else if (StringMatch(sk, VAR_PRECHST))
	{
		m_preChStorage[0] = m_preChStorage[m_outletID];
		*data = m_preChStorage;
	}
    else if (StringMatch(sk, VAR_SEEPAGE))
    {
        m_seepage[0] = m_seepage[m_outletID];
        *data = m_seepage;
    }
    else if (StringMatch(sk, VAR_CHWTDEPTH))
    {
        m_chWTdepth[0] = m_chWTdepth[m_outletID];
        *data = m_chWTdepth;
    }
	else if (StringMatch(sk, VAR_PRECHWTDEPTH))
	{
		m_preChWTDepth[0] = m_preChWTDepth[m_outletID];
		*data = m_preChWTDepth;
	}
	else if (StringMatch(sk, VAR_CHWTWIDTH))
	{
		m_chWTWidth[0] = m_chWTWidth[m_outletID];
		*data = m_chWTWidth;
	}
	else if (StringMatch(sk, VAR_CHBTMWIDTH))
	{
		m_chBtmWidth[0] = m_chBtmWidth[m_outletID];
		*data = m_chBtmWidth;
	}
    else
        throw ModelException(MID_MUSK_CH, "Get1DData", "Output " + sk+" does not exist.");
}

//! Get 2D data
void MUSK_CH::Get2DData(const char *key, int *nRows, int *nCols, float ***data)
{
    string sk(key);
    throw ModelException(MID_MUSK_CH, "Get2DData", "Parameter " + sk + " does not exist.");
}

////! Set 2D data
//void MUSK_CH::Set2DData(const char *key, int nrows, int ncols, float **data)
//{
//    string sk(key);
//    if (StringMatch(sk, Tag_RchParam))
//    {
//        m_nreach = ncols - 1;
//
//        m_reachId = data[0];
//        m_reachDownStream = data[1];
//        m_chOrder = data[2];
//        m_chWidth = data[3];
//        m_chLen = data[4];
//        m_chDepth = data[5];
//        m_chVel = data[6];
//        m_area = data[7];
//
//        m_reachUpStream.resize(m_nreach + 1);
//        if (m_nreach > 1)
//        {
//            for (int i = 1; i <= m_nreach; i++)// index of the reach is the equal to streamlink ID(1 to nReaches)
//            {
//                int downStreamId = int(m_reachDownStream[i]);
//                if (downStreamId <= 0)
//                    continue;
//                m_reachUpStream[downStreamId].push_back(i);
//            }
//        }
//    }
//    else
//        throw ModelException(MID_MUSK_CH, "Set2DData", "Parameter " + sk + " does not exist.");
//}
void MUSK_CH::SetScenario(Scenario * sce)
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
		throw ModelException(MID_MUSK_CH, "SetScenario", "The scenario can not to be NULL.");
}
void MUSK_CH::SetReaches(clsReaches *reaches)
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
			Initialize1DArray(m_nreach+1,m_area, 0.f);
			Initialize1DArray(m_nreach+1,m_chSideSlope, 2.f);
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
			m_area[i] = (float)tmpReach->GetArea();
			m_chSideSlope[i] = (float)tmpReach->GetSideSlope();
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
		throw ModelException(MID_MUSK_CH, "SetReaches", "The reaches input can not to be NULL.");
}
//! Get date time
void MUSK_CH::GetDt(float timeStep, float fmin, float fmax, float &dt, int &n)
{
    if (fmax >= timeStep)
    {
        dt = timeStep;
        n = 1;
        return;
    }

    n = int(timeStep / fmax);
    dt = timeStep / n;

    if (dt > fmax)
    {
        n++;
        dt = timeStep / n;
    }
}

//! Get coefficients
void MUSK_CH::GetCoefficients(float reachLength, float v0, MuskWeights &weights)
{
    v0 = m_vScalingFactor * v0;
    float K = (4.64f - 3.64f * m_co1) * reachLength / (5.f * v0 / 3.f);

    float min = 2.f * K * m_x;
    float max = 2.f * K * (1.f - m_x);
    float dt;
    int n;
    GetDt((float) m_dt, min, max, dt, n);
    weights.dt = dt;

    //get coefficient
    float temp = max + dt;
    weights.c1 = (dt - min) / temp;
    weights.c2 = (dt + min) / temp;
    weights.c3 = (max - dt) / temp;
    weights.c4 = 2 * dt / temp;
    weights.n = n;

    //make sure any coefficient is positive
    if (weights.c1 < 0)
    {
        weights.c2 += weights.c1;
        weights.c1 = 0.f;
    }
    if (weights.c3 < 0)
    {
        weights.c2 += weights.c1;
        weights.c3 = 0.f;
    }
}

void MUSK_CH::updateWaterWidthDepth(int i)
{
	/// update channel water depth and width according to channel water storage
	float crossArea = m_chStorage[i] / m_chLen[i];
	m_chWTdepth[i] = (sqrt(m_chBtmWidth[i]*m_chBtmWidth[i]+4.f*m_chSideSlope[i]*crossArea) - m_chBtmWidth[i])/2.f/m_chSideSlope[i];
	if (m_chWTdepth[i] < UTIL_ZERO)
		m_chWTWidth[i] = m_chBtmWidth[i];
	else
		m_chWTWidth[i] = m_chBtmWidth[i]+ 2.f * m_chSideSlope[i] * m_chWTdepth[i];
}
//! Channel flow
void MUSK_CH::ChannelFlow(int i)
{
    float st0 = m_chStorage[i];
    float qiSub = 0.f; /// interflow flow
    if (m_qiSub != NULL && m_qiSub[i] >= 0.f)
        qiSub = m_qiSub[i];
    float qgSub = 0.f; /// groundwater flow
    if (m_qgSub != NULL && m_qgSub[i] >= 0.f)
        qgSub = m_qgSub[i];
	float ptSub = 0.f; /// point sources flow
	if (m_ptSub != NULL && m_ptSub[i] >= 0.f)
		ptSub = m_ptSub[i];
    //////////////////////////////////////////////////////////////////////////
    // first add all the inflow water
    // 1. water from this subbasin

    float qIn = m_qsSub[i] + qiSub + qgSub + ptSub + m_deepGroundwater;  /// m^3
	//if (i == m_outletID) /// this should be added to each channel. By lj
	//	qIn += m_deepGroundwater;
	// 2. water from upstream reaches
    float qsUp = 0.f;
    float qiUp = 0.f;
    float qgUp = 0.f;
    for (size_t j = 0; j < m_reachUpStream[i].size(); ++j)
    {
        int upReachId = m_reachUpStream[i][j];
        qsUp += m_qsCh[upReachId];
        qiUp += m_qiCh[upReachId];
        qgUp += m_qgCh[upReachId];
    }
	// if (i == 8) cout << "qsUp:" << qsUp << ", qiUp:" << qiUp << ", qgUp:" << qgUp << ", m_qUpReach:" << m_qUpReach << endl;
    qIn += qsUp + qiUp + qgUp;
	//qIn is equivalent to the wtrin variable in rtmusk.f of SWAT
    qIn += m_qUpReach; // m_qUpReach is zero for not-parallel program and qsUp, qiUp and qgUp are zero for parallel computing
	//if(i == 12)
	//	cout <<"surfaceQ: "<< m_qsSub[i] << ", subsurfaceQ: " << qiSub << ", groundQ: " << qgSub << ", pointQ: " << ptSub <<
	//	", UPsurfaceQ: "<<qsUp<<", UPsubsurface: "<<qiUp<<", UPground: "<<qgUp<<", \n";
    // 3. water from bank storage
    float bankOut = m_bankStorage[i] * (1.f - exp(-m_aBank));

    m_bankStorage[i] -= bankOut;
    qIn += bankOut / m_dt;

    // add inflow water to storage
	//if (i==12) cout<<"initial chStorage: "<<m_chStorage[i]<<", ";
    m_chStorage[i] += qIn * m_dt;
	/// update channel water depth and width according to channel water storage
	updateWaterWidthDepth(i);
	//if (i==12) cout<<"added chStorage: "<<m_chStorage[i]<<endl;
	//if(i == 2) cout <<"qIn:"<< qIn<<", chStorage: "<<m_chStorage[i]<<endl;
    //////////////////////////////////////////////////////////////////////////
    // then subtract all the outflow water
    // 1. transmission losses to deep aquifer, which is lost from the system
    // the unit of kchb is mm/hr
    float seepage = m_Kchb / 1000.f / 3600.f * m_chBtmWidth[i] * m_chLen[i] * m_dt;
	//if(i == 2) cout << "seepage: " << seepage << endl;
    if (qgSub < UTIL_ZERO)
    {
        if (m_chStorage[i] > seepage)
        {
            m_seepage[i] = seepage;
            m_chStorage[i] -= seepage;
        }
        else
        {
            m_seepage[i] = m_chStorage[i];
            m_chStorage[i] = 0.f;
        }
    }
    else
    {
        m_seepage[i] = 0.f;
    }

    // 2. calculate transmission losses to bank storage
    //float dch = m_chStorage[i] / (m_chWTWidth[i] * m_chLen[i]);
	float dch = m_chWTdepth[i];
	float bankLen = dch * sqrt(1.f+m_chSideSlope[i]*m_chSideSlope[i]);
    float bankInLoss = 2.f * m_Kbank / 1000.f / 3600.f * bankLen * m_chLen[i] * m_dt;   // m^3
    bankInLoss = 0.f; //TODO
    if (m_chStorage[i] > bankInLoss)
    {
        m_chStorage[i] -= bankInLoss;
    }
    else
    {
        bankInLoss = m_chStorage[i];
        m_chStorage[i] = 0.f;
    }
    // water balance of the bank storage
    // loss the water from bank storage to the adjacent unsaturated zone and groundwater storage
    float bankOutGw = m_bankStorage[i] * (1.f - exp(-m_bBank));
    bankOutGw = 0.f; //TODO
    m_bankStorage[i] = m_bankStorage[i] + bankInLoss - bankOutGw;
    if (m_gwStorage != NULL)
        m_gwStorage[i] += bankOutGw / m_area[i] * 1000.f;   // updated groundwater storage

    // 3. evaporation losses
    float et = 0.f;
    if (m_petCh != NULL)
    {
        et = m_Epch * m_petCh[i] / 1000.0f * m_chWTWidth[i] * m_chLen[i];    //m3
        if (m_chStorage[i] > et)
        {
            m_chStorage[i] -= et;
        }
        else
        {
            et = m_chStorage[i];
            m_chStorage[i] = 0.f;
        }
    }
	if (FloatEqual(m_chStorage[i], 0.f))
	{
		m_qOut[i] = 0.f;
		m_qsCh[i] = 0.f;
		m_qiCh[i] = 0.f;
		m_qgCh[i] = 0.f;
		m_chWTdepth[i] = 0.f;
		m_chWTWidth[i] = 0.f;
		return;
	}

	//if(i == 2) cout << "chStorage before routing " << m_chStorage[i] << endl;
    //////////////////////////////////////////////////////////////////////////
    // routing, there are water in the channel after inflow and transmission loss
	float totalLoss = m_seepage[i] + bankInLoss + et;
	//if(i == 12) cout << ",  m_petCh: " << m_petCh[i] << ",  et: " << et << ",  m_chStorage: " << m_chStorage[i] << ", \n";
    //if (m_chStorage[i] >= 0.f)
    //{
//         qIn -= totalLoss / m_dt;// average loss rate during m_dt
// 		if(qIn < 0.f) qIn = 0.f;

	m_preChStorage[i] = m_chStorage[i];
	m_preChWTDepth[i] = m_chWTdepth[i];
        m_chStorage[i] = st0;

        // calculate coefficients
        MuskWeights wt;
        GetCoefficients(m_chLen[i], m_chVel[i], wt);
        int n = wt.n;
        float q = 0.f;
        for (int j = 0; j < n; j++)
        {
            m_qOut[i] = wt.c1 * qIn + wt.c2 * m_qIn[i] + wt.c3 * m_qOut[i];
            m_qIn[i] = qIn;
            float tmp = m_chStorage[i] + (qIn - totalLoss/m_dt - m_qOut[i]) * wt.dt;
            if (tmp < 0.f)
            {
                m_qOut[i] = m_chStorage[i] / wt.dt + qIn;
                m_chStorage[i] = 0.f;
            }
            else
            {
                m_chStorage[i] = tmp;
            }
            q += m_qOut[i];
        }
        m_qOut[i] = q / n;
    //}
    //else
    //{
    //    m_qOut[i] = 0.f;
    //    m_chStorage[i] = 0.f;
    //    qIn = 0.f;
    //}
    float qInSum = m_qsSub[i] + qiSub + qgSub + qsUp + qiUp + qgUp;
    m_qsCh[i] = m_qOut[i] * (m_qsSub[i] + qsUp) / qInSum;
    m_qiCh[i] = m_qOut[i] * (qiSub + qiUp) / qInSum;
    m_qgCh[i] = m_qOut[i] * (qgSub + qgUp) / qInSum;

    // set variables for next time step
    m_qIn[i] = qIn;
    //m_chWTdepth[i] = m_chStorage[i] / (m_chWTWidth[i] * m_chLen[i]);
	updateWaterWidthDepth(i);
}
