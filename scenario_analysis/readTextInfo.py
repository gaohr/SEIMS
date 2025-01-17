# -*- coding: utf-8 -*-
# @Read BMP_Scenario text info
# @Author Huiran GAO
# @Date   2016-10-28

import os
import datetime
import time
import random
import platform
import scoop
import matplotlib.pyplot as plt
from pymongo import MongoClient
from RelativeImportModules import *
import numpy
from util import *
# # import util module located in SEIMS/preprocess
# if __package__ is None:
#     __package__ = import_parents(level=2)
# from ..preprocess.util import *

def delSpecialStr(line):
    '''
    :param line:
    :return: line that remove ['\r\n', '\n\r', '\r', '\n']
    '''
    LFs = ['\r\n', '\n\r', '\r', '\n']
    for LF in LFs:
        if LF in line:
            line = line.split(LF)[0]
            break
    return line


###### farms ######
def getFieldInfo(fieldFile):
    '''
    :param fieldFile: full path of field text file
    :return: field number, field list
    '''
    # Get field info
    fieldTtextArr = []
    if os.path.isfile(fieldFile):
        fieldfile_object = open(fieldFile, "r")
        try:
            li = 0
            for line in fieldfile_object:
                line = delSpecialStr(line)
                if li != 0 and li != 2:
                    fieldTtextArr.append(line.strip())
                li += 1
        finally:
            fieldfile_object.close()

    # Get farm field index
    fieldsNum = int(fieldTtextArr[0])
    farmFields = []
    farmLU = []
    farm_area = []
    for i in range(1, fieldsNum):
        fieldInfo = fieldTtextArr[i].split('\t')
        if int(fieldInfo[3]) == 1 or int(fieldInfo[3]) == 33:
            farmFields.append(int(fieldInfo[0]))
            farm_area.append(float(fieldInfo[2]) * 15.)  # ha -> mu
            farmLU.append(int(fieldInfo[3]))
    # print farmFields
    return fieldsNum, farmFields, farmLU, farm_area


def getPointSource(pointFile):
    '''
    :param pointFile: full path of point source distribute file
    :return: cattle points list, pig points list, sewage points list
    '''
    ###### points ######
    pointTextArr = []
    point_cattle = []
    point_pig = []
    point_sewage = []
    point_cattle_size = []
    point_pig_size = []
    point_sewage_size = []
    if os.path.isfile(pointFile):
        pointfile_object = open(pointFile, "r")
        try:
            for line in pointfile_object:
                line = delSpecialStr(line)
                if len(line) > 0:
                    if line[0] is not "#":
                        pointTextArr.append(line.strip())
                        # del pointTtextArr[0]
        finally:
            pointfile_object.close()
    # Get animal farms index
    for i in range(1, len(pointTextArr)):
        pointInfo = pointTextArr[i].split('\t')
        if int(pointInfo[0]) == 10000:
            point_cattle.append(pointInfo[1])
            point_cattle_size.append(int(pointInfo[7]))
        elif int(pointInfo[0]) == 20000:
            point_pig.append(pointInfo[1])
            point_pig_size.append(int(pointInfo[7]))
        else:
            point_sewage.append(pointInfo[1])
            point_sewage_size.append(int(pointInfo[7]))
    return point_cattle, point_pig, point_sewage, point_cattle_size, point_pig_size, point_sewage_size


def getBMPsInfo(pointBMPsFile, pointFile):
    '''
    :param pointBMPsFile: full path of points BMPs info file
    :return: farm BMPs, cattle BMPs, pig BMPs, sewage BMPs
    '''
    # Get BMPs info
    # BMPs_farm_id = []
    # BMPs_farm = []
    BMPs_cattle_id = []
    BMPs_cattle = []
    BMPs_pig_id = []
    BMPs_pig = []
    BMPs_sewage_id = []
    BMPs_sewage = []

    pointbmpsTextArr = []
    if os.path.isfile(pointBMPsFile):
        pointbmpsfile_object = open(pointBMPsFile, "r")
        try:
            for line in pointbmpsfile_object:
                line = delSpecialStr(line)
                if len(line) > 0:
                    if line[0] is not "#":
                        pointbmpsTextArr.append(line.strip())
        finally:
            pointbmpsfile_object.close()

    # Get animal farms' BMPs info
    for j in range(1, len(pointbmpsTextArr)):
        pointbmpsInfo = pointbmpsTextArr[j].split('\t')
        if int(float(pointbmpsInfo[0]) / 10000.) == 1:
            BMPs_cattle_id.append(pointbmpsInfo[0])
        elif int(float(pointbmpsInfo[0]) / 10000.) == 2:
            BMPs_pig_id.append(pointbmpsInfo[0])
        else:
            BMPs_sewage_id.append(pointbmpsInfo[0])

    BMPs_farm = [0, 1]
    for cc in range(len(BMPs_cattle_id)):
        BMPs_cattle.append(int(BMPs_cattle_id[cc]) - 10000)
    for pp in range(len(BMPs_pig_id)):
        BMPs_pig.append(int(BMPs_pig_id[pp]) - 20000)
    for ss in range(len(BMPs_sewage_id)):
        BMPs_sewage.append(int(BMPs_sewage_id[ss]) - 40000)

    # BMP cost
    # BMPs_farm_cost = []
    BMPs_cattle_cost = numpy.zeros((len(BMPs_cattle) + 1))
    BMPs_pig_cost = numpy.zeros((len(BMPs_pig) + 1))
    BMPs_sewage_cost = numpy.zeros((len(BMPs_sewage) + 1))

    pt_swg_size = numpy.zeros((len(BMPs_sewage) + 1))
    pt_swg = getPointSource(pointFile)[2]
    pt_swg_size_tmp = getPointSource(pointFile)[5]
    for p_s in range(len(pt_swg)):
        p_s_index = int(float(pt_swg[p_s]) % 10000.) - 1
        pt_swg_size[p_s_index] = pt_swg_size_tmp[p_s]
        pt_swg_size[p_s_index + 4] = pt_swg_size_tmp[p_s]

    for j in range(1, len(pointbmpsTextArr)):
        pointbmpsInfo = pointbmpsTextArr[j].split('\t')
        q_pollution = float(pointbmpsInfo[10])
        CAPEX = float(pointbmpsInfo[20])
        OPEX = float(pointbmpsInfo[21])
        INCOME = float(pointbmpsInfo[22])
        if int(float(pointbmpsInfo[0]) / 10000.) == 1:
            cost = CAPEX + OPEX - INCOME
            bmp_ctco_index = int(float(pointbmpsInfo[0]) - 10000.)
            BMPs_cattle_cost[bmp_ctco_index] = cost
        elif int(float(pointbmpsInfo[0]) / 10000.) == 2:
            cost = CAPEX + OPEX - INCOME
            bmp_pgco_index = int(float(pointbmpsInfo[0]) - 20000.)
            BMPs_pig_cost[bmp_pgco_index] = cost
        else:
            bmp_swgco_index = int(float(pointbmpsInfo[0]) - 40000.)
            # print q_pollution, pt_swg_size[bmp_swgco_index - 1]
            cost = CAPEX + OPEX * q_pollution * pt_swg_size[bmp_swgco_index - 1] * 365 - INCOME
            BMPs_sewage_cost[bmp_swgco_index] = cost

    # print BMPs_farm, '\n', BMPs_cattle, '\n', BMPs_pig, '\n', BMPs_sewage, '\n', \
    #         BMPs_cattle_cost, '\n', BMPs_pig_cost, '\n', BMPs_sewage_cost
    return BMPs_farm, BMPs_cattle, BMPs_pig, BMPs_sewage, BMPs_cattle_cost, BMPs_pig_cost, BMPs_sewage_cost


def selectBMPatRandom(arr):
    '''
    :param arr: BMPs list
    :return: select a BMP randomly
    '''
    aLen = len(arr)
    n = random.randint(0, aLen - 1)
    return arr[n]

def getFarmConfig(farm_scenario, field):
    '''
    :param farm_scenario: farm scenario
    :param feild:
    :return:
    '''
    farmConfig = []
    bmpdo = ""
    bmpundo = ""
    for i in range(len(farm_scenario)):
        if farm_scenario[i] == 0:
            bmpundo += str(field[i]) + ","
        else:
            bmpdo += str(field[i]) + ","
    farmConfig.append(bmpundo[:-1])
    farmConfig.append(bmpdo[:-1])
    return farmConfig


def getPointConfig(scenario, bmps_point, point_source, start_index, end_index):
    '''
    :param scenario: scenario array
    :param bmps_point: BMPs list of point
    :param point_source: point source list
    :param start_index: point source start index in scenario array
    :param end_index: point source end index in scenario array
    :return: config info array
    '''
    n_index = end_index - start_index
    if n_index > 0:
        pointConfig = []
        for bc in range(len(bmps_point)):
            bmp_index = []
            bmp_index.append(bmps_point[bc])
            for c in range(start_index, end_index):
                if scenario[c] == bmps_point[bc]:
                    bmp_index.append(point_source[c - start_index])
                else:
                    continue
            pointConfig.append(bmp_index)
        return pointConfig
    else:
        return []


def decodPointScenario(id, pointConfig, ptsrc):
    '''
    :param pointConfig: config info array obtained from function getPointConfig()
    :return: part of BMPs scenarios text
    '''
    scenario_Table = []
    for config in pointConfig:
        if len(config) > 1:
            scenario_Row = ""
            scenario_Row += str(id) + "\tsName" + str(id) + "\t1\t" + str(ptsrc + config[0]) \
                            + "\tARRAY|POINT_SOURCE_DISTRIBUTION|" + str(ptsrc) + "\tPOINT_SOURCE_MANAGEMENT\t"
            pidArr = ""
            for pid in range(1, len(config)):
                if pid == len(config) - 1:
                    pidArr += config[pid]
                else:
                    pidArr += config[pid] + ","
            scenario_Row += pidArr
            scenario_Table.append(scenario_Row)
    return scenario_Table


def ReadSimfromTxt(timeStart, timeEnd, dataDir, sim, subbasinID=0):
    TIME_Start = datetime.datetime.strptime(timeStart, "%Y-%m-%d")
    TIME_End = datetime.datetime.strptime(timeEnd, "%Y-%m-%d")
    ## Read simulation txt
    simData = "%s/%d_%s.txt" % (dataDir, subbasinID, sim)
    while not os.path.isfile(simData):
        time.sleep(1)
    # whether the text file existed?
    if not os.path.isfile(simData):
        raise IOError("%s is not existed, please check the configuration!" % simData)
    simulate = []
    LFs = ['\r\n', '\n\r', '\r', '\n']
    if os.path.exists(simData):
        simfile = open(simData, "r")
        while True:
            line = simfile.readline()
            # print line[0]
            if line:
                for LF in LFs:
                    if LF in line:
                        line = line.split(LF)[0]
                        break
                strList = SplitStr(StripStr(line), spliters=" ")
                if len(strList) == 3:
                    dateStr = strList[0] + " " + strList[1]
                    simDatetime = datetime.datetime.strptime(dateStr, "%Y-%m-%d %H:%M:%S")
                    if simDatetime >= TIME_Start and simDatetime <= TIME_End:
                        simulate.append(float(strList[2]))
            else:
                break
        simfile.close()
        # print simulate
        return simulate
    else:
        raise IOError("%s is not exist" % simData)

# String Array to float/int Array
def StrtoFltArr(arr):
    newArr = []
    for i in range(len(arr)):
        newArr.append(float(arr[i]))
    return newArr

def StrtoIntArr(arr):
    newArr = []
    for i in range(len(arr)):
        newArr.append(int(arr[i]))
    return newArr

def printInfo(Str):
    if platform.system() is "Windows":
        print Str
    else:
        scoop.logger.warn(Str)

def createForld(forldPath):
    ## Create forld
    if not isPathExists(forldPath):
        os.makedirs(forldPath)

def createPlot(pop, model_Workdir, num_Gens, size_Pops, GenID):
    front = numpy.array([ind.fitness.values for ind in pop])
    # Plot
    plt.figure(GenID)
    plt.title("Pareto frontier of Scenarios Optimization\n", color="#aa0903")
    plt.xlabel("Economic cost(Million Yuan)")
    plt.ylabel("Pollution load(t)")
    front[:, 0] /= 1000000.
    front[:, 1] /= 1000.
    plt.scatter(front[:, 0], front[:, 1], c="r", alpha=0.8, s=12)
    plt.title("\nPopulation: %d, Generation: %d" % (size_Pops, GenID), color="green", fontsize=9, loc='right')
    imgPath = model_Workdir + os.sep + "NSGAII_OUTPUT"
    createForld(imgPath)
    pngFullpath = imgPath + os.sep + "Gen_" \
                  + str(num_Gens) + "_Pop_" + str(size_Pops)+ os.sep + "Pareto_Gen_" \
                  + str(GenID) + "_Pop_" + str(size_Pops) + ".png"
    plt.savefig(pngFullpath)
    # plt.show()

def getSceIDlist(scenarios_info):
    # Get scenarios info
    fieldTtextArr = []
    if os.path.isfile(scenarios_info):
        fieldfile_object = open(scenarios_info, "r")
        for line in fieldfile_object:
            fieldTtextArr.append(line.strip())
    idlist = []
    for i in range(0, len(fieldTtextArr)):
        id = fieldTtextArr[i].split('\t')[0]
        idlist.append(id)
    return idlist

def delScefromMongoByID(scenarios_info, hostname, port, dbname, delsce=True):
    if delsce == True:
        idList = getSceIDlist(scenarios_info)
        client = MongoClient(hostname, port)
        db = client[dbname]
        collection = db.BMP_SCENARIOS
        for id in idList:
            collection.remove({"ID":id})
    else:
        return

def delModelOutfile(model_workdir, scenarios_info, delfile=True):
    if delfile == True:
        idList = getSceIDlist(scenarios_info)
        # print idList
        for id in idList:
            outfilename = model_workdir + os.sep + "OUTPUT" + str(id)
            fileList = os.listdir(outfilename)
            for f in fileList:
                filename = outfilename + os.sep + str(f)
                if os.path.isfile(filename):
                    os.remove(filename)
            if os.path.isdir(outfilename):
                os.rmdir(outfilename)
    else:
        return


# if __name__ == "__main__":
#     fieldFile = r'D:\GaohrWS\GithubPrj\SEIMS\model_data\dianbu\data_prepare\spatial\mgtfield_t100.txt'
#     pointFile = r'D:\GaohrWS\GithubPrj\SEIMS\model_data\dianbu\data_prepare\management\point_source_distribution.txt'
#     pointBMPsFile = r'D:\GaohrWS\GithubPrj\SEIMS\model_data\dianbu\data_prepare\management\point_source_management.txt'
    # Scenario
    # fieldInfo = getFieldInfo(fieldFile)
    # pointSource = getPointSource(pointFile)
    # bmpsInfo = getBMPsInfo(pointBMPsFile, pointFile)
    # field_farm = fieldInfo[1]
    # field_lu = fieldInfo[2]
    # farm_area = fieldInfo[3]
    # point_cattle = pointSource[0]
    # point_pig = pointSource[1]
    # point_sewage = pointSource[2]
    # farm_Num = len(field_farm)
    # farm_Num = 1
    # point_cattle_Num = len(point_cattle)
    # point_pig_Num = len(point_pig)
    # point_sewage_Num = len(point_sewage)
    # BMP cost
    # bmps_cattle_cost = bmpsInfo[4]
    # bmps_pig_cost = bmpsInfo[5]
    # bmps_sewage_cost = bmpsInfo[6]
    # livestock farm size
    # point_cattle_size = pointSource[3]
    # point_pig_size = pointSource[4]

    # print 'field_farm ', field_farm
    # print 'field_lu ', field_lu
    # print 'farm_area ', farm_area
    # print 'point_cattle ', point_cattle
    # print 'point_pig ', point_pig
    # print 'point_sewage ', point_sewage
    # print 'bmps_cattle_cost ', bmps_cattle_cost
    # print 'bmps_pig_cost ', bmps_pig_cost
    # print 'bmps_sewage_cost ', bmps_sewage_cost
    # print 'point_cattle_size ', point_cattle_size
    # print 'point_pig_size ', point_pig_size

