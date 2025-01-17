/*----------------------------------------------------------------------
*	Purpose: 	Channel flow sediment bound nutrient
*
*	Created:	Wang Lin
*	Date:		2-Aug-2012
*---------------------------------------------------------------------*/

#include "api.h"
#include <string>
#include <ctime>
#include <cmath>
#include <map>
#include <vector>
#include "SimulationModule.h"

using namespace std;

class SedBonNutrient_CH : public SimulationModule
{
public:
    SedBonNutrient_CH(void);

    ~SedBonNutrient_CH(void);

    virtual int Execute();

    virtual void SetValue(const char *key, float value);

    virtual void GetValue(const char *key, float *value);

    virtual void Set1DData(const char *key, int n, float *data);

    virtual void Get1DData(const char *key, int *n, float **data);

    virtual void Set2DData(const char *key, int nrows, int ncols, float **data);

    virtual void Get2DData(const char *key, int *nRows, int *nCols, float ***data);

    bool CheckInputSize(const char *key, int n);

    bool CheckInputData(void);

private:
    //Parameters
    map<int, vector<int>> m_reachLayers;
    /**
    *	@brief 2d array of flow in cells
    *
    *	The first element in each sub-array is the number of flow in cells in this sub-array
    */
    float **m_flowInIndex;
    /// flow out index
    float *m_flowOutIndex;
    // id the reaches
    float *m_reachId;
    /// stream order
    float *m_streamOrder;
    /// stream id of downstream
    float *m_reachDownStream;
    /// upstream id (The value is -1 if there if no upstream reach)
    vector<vector<int> > m_reachUpStream;
    /**
    *	@brief reach links
    *
    *	key: index of the reach
    *	value: vector of cell index
    */
    map<int, vector<int> > m_reachs;
    /// cell width of the grid (m)
    float m_CellWidth;
    /// time step (second)
    float m_dt;
    /// number of valid cells
    int m_nCells;
    /// id of source cells of reaches
    int *m_sourceCellIds;
    /// channel number
    int m_chNumber;
    /// stream link
    float *m_streamLink;
    /// map from subbasin id to index of the array
    map<int, int> m_idToIndex;

    /// slope of map, to calculate slope gradient.
    float *m_Slope;
    /// channel width (zero for non-channel cells)
    float *m_chWidth;
    /// water depth for channel cell [mm]
    float **m_ChannelWH;
    /// water volume in cell m3
    float **m_ChVol;
    /// channel flow of each cell, ��m3/s��
    float **m_ChQkin;

    /// sediment bound P flow to channel (kg)
    float *m_SedBonPToCh;
    /// sediment bound ammonium flow to channel (kg)
    float *m_SedBonAmmoniumToCh;

    //output
    /// distribution of sediment bound P (kg/s) in channel
    float **m_SedBonP;
    /// distribution of nutrient, sediment bound ammonium (kg/s) in channel
    float **m_SedBonAmmonium;
    /// sediment bound P flow to outlet (kg)
    float **m_SedBonP_KG;
    /// sediment bound ammonium flow to outlet (kg)
    float **m_SedBonAmmonium_KG;


    /// the sum of the initial values of the nutrient specie (kg/s)
    float *m_PPT_P;
    float *m_PPT_NH4;
    float *m_PPT_NO3;
    /// concentration of sediment bound P
    float *m_CP_Sed;
    /// concentration of sediment bound NH4
    float *m_CNH4_Sed;
    /// flow width of each cell
    float *m_flowWidth;
    /// flow length of each cell
    float *m_flowLen;
    ///  the ratio of runoff and storage volume
    float m_X1;
    ///  the ratio of runoff plus infiltration and storage volume
    float m_X2;
    /// the ratio of runoff and storage volume
    float m_X3;
    /// cell area m2
    float *m_cellA;

private:

    static string toString(float value);

    void initialOutputs();

    void GetNutrientInFlow(int iReach, int iCell, int id);

    /**
    *	@brief Simple calculation of sediment outflux from a cell based on the sediment concentration multiplied by the new water flux,
    *	j = time and i = place: j1i1 is the new output, j1i is the new flux at the upstream 'entrance' flowing into the gridcell
    *	@param  Qn  result kin wave for this cell
    *	@param  Qin  sum of all upstreamwater from kin wave
    *	@param  Sin  sum of all upstream sediment   kg/s
    *	@param  dt    the time step
    *	@param  vol   current volume of water in cell
    *	@param  sed    current mass of sediment in cell
    *	@return the newer sediment outflux, kg/s
    */
    float simpleSedCalc(float Qj1i1, float Qj1i, float Sj1i, float dt, float vol, float sed);

    void WaterVolumeCalc(int iReach, int iCell, int id);  //m3

};