# -*- coding: utf-8 -*-
# @Class Scenario
# @Author Huiran GAO
# @Date   2016-10-29

import os, sys
import random
import time
import scoop
from pymongo import MongoClient
from subprocess import Popen
from subprocess import PIPE
from config import *
from readTextInfo import *


class Scenario:
    def __init__(self):
        self.id = None
        self.attributes = []
        self.field_Num = farm_Num
        self.point_cattle_Num = point_cattle_Num
        self.point_pig_Num = point_pig_Num
        self.point_sewage_Num = point_sewage_Num
        self.sce_list = []
        self.cost_eco = 0.
        self.benefit_env = 0.

    def getIdfromMongo(self):
        '''
        set new scenario id according to the existing
         scenario ids, i.e., the max id + 1
        '''
        client = MongoClient(HOSTNAME, PORT)
        db = client[BMPScenarioDBName]
        collection = db.BMP_SCENARIOS
        idsList = []
        for s in collection.find():
            idsList.append(int(s['ID']))
        idList = list(set(idsList))
        self.id = idList[-1] + 1

    def setId(self, id):
        '''
        set new scenario id by given number
        :param id:
        '''
        self.id = id

    def create(self):
        # Create a scenario numeric string
        for _ in range(self.field_Num):
            self.attributes.append(selectBMPatRandom(bmps_farm))
        for _ in range(self.point_cattle_Num):
            self.attributes.append(selectBMPatRandom(bmps_cattle))
        for _ in range(self.point_pig_Num):
            self.attributes.append(selectBMPatRandom(bmps_pig))
        for pt_swg in range(self.point_sewage_Num):
            p_s_index = int(point_sewage[pt_swg]) % 10000 - 1
            self.attributes.append(selectBMPatRandom(bmps_sewage[p_s_index]))

    def decoding(self):
        # scenario section
        if len(self.attributes) == 0:
            raise Exception("'attributes' cannot be Null!")
        if self.id is None:
            raise Exception("'id' cannot be None!")
        field_index = self.field_Num
        point_cattle_index = self.point_cattle_Num + field_index
        point_pig_index = self.point_pig_Num + point_cattle_index
        point_sewage_index = self.point_sewage_Num + point_pig_index
        # farm field
        sinfo = str(self.id) + "\tS_" + str(self.id) + "\t12\t"
        farm_BMP_dedupl = list(set(self.attributes[0:field_index]))
        if len(farm_BMP_dedupl) > 1:
            for f1 in range(4):
                scenario_Row = ""
                scenario_Row += sinfo
                farm_conf = getFarmConfig(self.attributes[0:field_index], field_farm)
                if f1 == 0 or f1 == 1:
                    scenario_Row += str(f1) + "\tRASTER|MGT_FIELDS\tPLANT_MANAGEMENT\t" + farm_conf[0]
                else:
                    scenario_Row += str(f1) + "\tRASTER|MGT_FIELDS\tPLANT_MANAGEMENT\t" + farm_conf[1]
                self.sce_list.append(scenario_Row)
        else:
            for f2 in range(2):
                scenario_Row = ""
                scenario_Row += sinfo
                farm_BMP_do = False
                for i in range(0, field_index):
                    if self.attributes[i] == 1:
                        farm_BMP_do = True
                    else:
                        farm_BMP_do = False
                if farm_BMP_do:
                    scenario_Row += str(bmps_farm[f2] + 2) + "\t"
                else:
                    scenario_Row += str(bmps_farm[f2]) + "\t"
                scenario_Row += "RASTER|MGT_FIELDS\tPLANT_MANAGEMENT\tALL"
                self.sce_list.append(scenario_Row)
        # point source
        # Cattle
        cattleConfig = getPointConfig(self.attributes, bmps_cattle, point_cattle, field_index, point_cattle_index)
        if len(cattleConfig) > 0:
            self.sce_list.extend(decodPointScenario(self.id, cattleConfig, 10000))
        pigConfig = getPointConfig(self.attributes, bmps_pig, point_pig, point_cattle_index, point_pig_index)
        # Pig
        if len(pigConfig) > 0:
            self.sce_list.extend(decodPointScenario(self.id, pigConfig, 20000))
        # Sewage
        for p_s in range(self.point_sewage_Num):
            s_index = int(int(point_sewage[p_s]) % 10000)
            sewageConfig = getPointConfig(self.attributes, bmps_sewage[s_index], point_sewage, point_pig_index, point_sewage_index)
            if len(sewageConfig) > 0:
                self.sce_list.extend(decodPointScenario(self.id, sewageConfig, 40000))

    def importoMongo(self, hostname, port, dbname):
        '''
        Import scenario list to MongoDB
        :return:
        '''
        client = MongoClient(hostname, port)
        db = client[dbname]
        collection = db.BMP_SCENARIOS
        keyarray = ["ID", "NAME", "BMPID", "SUBSCENARIO", "DISTRIBUTION", "COLLECTION", "LOCATION"]
        for line in self.sce_list:
            conf = {}
            li_list = line.split('\t')
            for i in range(len(li_list)):
                if isNumericValue(li_list[i]):
                    conf[keyarray[i]] = float(li_list[i])
                else:
                    conf[keyarray[i]] = str(li_list[i]).upper()
            collection.insert(conf)

    def cost(self):
        if len(self.attributes) == 0:
            raise Exception("'attributes' cannot be Null!")
        field_index = self.field_Num
        point_cattle_index = self.point_cattle_Num + field_index
        point_pig_index = self.point_pig_Num + point_cattle_index
        point_sewage_index = self.point_sewage_Num + point_pig_index
        for i1 in range(0, field_index):
            self.cost_eco += bmps_farm_cost[int(self.attributes[i1])] * farm_area[i1]
        for i2 in range(field_index, point_cattle_index):
            self.cost_eco += bmps_cattle_cost[int(self.attributes[i2])] * point_cattle_size[i2 - field_index]
        for i3 in range(point_cattle_index, point_pig_index):
            self.cost_eco += bmps_pig_cost[int(self.attributes[i3])] * point_pig_size[i3 - point_cattle_index]
        for i4 in range(point_pig_index, point_sewage_index):
            self.cost_eco += bmps_sewage_cost[int(self.attributes[i4])]

    def benefit(self):
        printInfo("Scenario ID: " + str(self.id))
        startT = time.time()
        cmdStr = "%s %s %d %d %s %d %d" % (
        model_Exe, model_Workdir, threadsNum, layeringMethod, HOSTNAME, PORT, self.id)
        # print cmdStr
        process = Popen(cmdStr, shell=True, stdout=PIPE)
        while process.stdout.readline() != "":
            line = process.stdout.readline().split("\n")
            if line[0] != "" and len(line[0]) == 20:
                lineArr = line[0].split(' ')[0].split('-')
                if int(lineArr[2]) == 1:
                    sys.stdout.write(str(lineArr[0]) + "-" + str(lineArr[1]) + " ")
            continue
        process.wait()
        time.sleep(1)
        if process.returncode == 0:
            # if True:
            dataDir = model_Workdir + os.sep + "OUTPUT" + str(self.id)
            polluteList = ['CH_COD', 'CH_TN', 'CH_TP']
            # polluteList = ['CH_COD']
            polluteWt = [33., 4., 1.]
            # polluteWt = [1.]
            for pp in range(len(polluteList)):
                simData = ReadSimfromTxt(timeStart, timeEnd, dataDir, polluteList[pp], subbasinID=0)
                self.benefit_env += sum(simData) / polluteWt[pp]
        # print self.benefit_env
        printInfo("cost_eco: " + str(self.cost_eco))
        printInfo("benefit_env: " + str(self.benefit_env))
        endT = time.time()
        printInfo("SEIMS running time: %.2fs" % (endT - startT))

    def saveInfo(self, txtfile):
        outfile = file(txtfile, 'a')
        infoStr = str(self.id) + "\t" + str(self.cost_eco) + "\t" + str(self.benefit_env) \
                  + "\t" + str(self.attributes) + LF
        outfile.write(infoStr)
        outfile.close()


if __name__ == "__main__":
    s = Scenario()
    s.id = 1
    s.attributes = numpy.array([1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 3.0, 3.0, 3.0, 2.0, 3.0, 2.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 2.0, 2.0, 3.0, 2.0, 3.0, 2.0, 3.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 3.0])
    s.decoding()
#     s.cost()
    print s.sce_list
