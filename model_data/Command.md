## TEST COMMANDS BY ZHULJ:

### Preprocess-Python Scripts

+ `python C:\z_code\hydro\SEIMS\preprocess\main.py -ini C:\z_code\hydro\SEIMS\preprocess\dianbu_30m_longterm_omp_zhulj_win.ini`
+ `python C:\z_code\hydro\SEIMS\preprocess\import_parameters.py -ini C:\z_code\hydro\SEIMS\preprocess\dianbu_30m_longterm_omp_zhulj_win.ini`
+ `python C:\z_code\hydro\SEIMS\preprocess\parameters_extraction.py -ini C:\z_code\hydro\SEIMS\preprocess\dianbu2_30m_longterm_omp_zhulj_win.ini`
+ `python C:\z_code\hydro\SEIMS\preprocess\build_db.py -ini C:\z_code\hydro\SEIMS\preprocess\dianbu2_30m_longterm_omp_zhulj_win.ini`
+ `python C:\z_code\hydro\SEIMS\preprocess\import_bmp_scenario.py -ini C:\z_code\hydro\SEIMS\preprocess\dianbu_30m_longterm_omp_zhulj_win.ini`
+ `python C:\z_code\hydro\SEIMS\preprocess\generate_stream_input.py -ini C:\z_code\hydro\SEIMS\preprocess\dianbu_30m_longterm_omp_zhulj_win.ini`
+ `python C:\z_code\hydro\SEIMS\preprocess\import_measurement.py -ini C:\z_code\hydro\SEIMS\preprocess\dianbu_30m_longterm_omp_zhulj_win.ini`

### Preprocess-C++ Programs

+ `D:\Compile\SEIMS_Preprocess\Release\grid_layering 127.0.0.1 27017 C:\z_data_m\SEIMS\dianbu2_30m_omp\layering_info model_dianbu2_30m_longterm SPATIAL 0`

+ `D:\Compile\SEIMS_Preprocess\Release\iuh 127.0.0.1 27017 model_dianbu_30m_longterm SPATIAL 24 1`

+ `D:\Compile\SEIMS_Preprocess\Release\iuh 192.168.6.55 27017 model_dianbu_30m_longterm SPATIAL 24 1`

### Run SEIMS model

+ `D:\Compile\SEIMS_OMP\Release\seims_omp C:\z_code\Hydro\SEIMS\model_data\dianbu\model_dianbu2_30m_longterm 6 0 192.168.6.55 27017 24217612`

+ `D:\Compile\SEIMS_OMP\Release\seims_omp C:\z_code\Hydro\SEIMS\model_data\dianbu\model_dianbu_30m_longterm 6 0 127.0.0.1 27017 0`

### PostProcess-Python Scripts

+ `python C:\z_code\Hydro\SEIMS\postprocess\hydroPlot_main.py -ini C:\z_code\Hydro\SEIMS\postprocess\post_dianbu_30m_longterm_omp_zhulj_win.ini`


## Case study: dingguang watershed

+ `python C:\z_code\hydro\SEIMS\preprocess\main.py -ini C:\z_code\hydro\SEIMS\preprocess\dingguang_30m_longterm_omp_zhulj_win.ini`
+ `python C:\z_code\hydro\SEIMS\preprocess\import_parameters.py -ini C:\z_code\hydro\SEIMS\preprocess\dingguang_30m_longterm_omp_zhulj_win.ini`
+ `python C:\z_code\hydro\SEIMS\preprocess\import_bmp_scenario.py -ini C:\z_code\hydro\SEIMS\preprocess\dingguang_30m_longterm_omp_zhulj_win.ini`
+ `python C:\z_code\hydro\SEIMS\preprocess\subbasin_delineation.py -ini C:\z_code\hydro\SEIMS\preprocess\dingguang_30m_longterm_omp_zhulj_win.ini`
+ `python C:\z_code\hydro\SEIMS\preprocess\parameters_extraction.py -ini C:\z_code\hydro\SEIMS\preprocess\dingguang_30m_longterm_omp_zhulj_win.ini`
+ `D:\Compile\SEIMS_Preprocess\Release\fieldpartition C:\z_data_m\SEIMS\field_partition\dingguang 100`
+ `D:\Compile\SEIMS_OMP\Release\seims_omp C:\z_code\hydro\SEIMS\model_data\dingguang\model_dingguang_30m_longterm 6 0 127.0.0.1 27017 0`
+ `python C:\z_code\Hydro\SEIMS\postprocess\hydroPlot_main.py -ini C:\z_code\Hydro\SEIMS\postprocess\post_dingguang_30m_longterm_omp_zhulj_win.ini`

+ `D:\Compile\SEIMS_Preprocess\Release\grid_layering 127.0.0.1 27017 C:\z_data_m\SEIMS\dingguang_30m_omp\layering_info model_dingguang_30m_longterm SPATIAL 0`

+ `G:\compile\SEIMS\Release\seims_omp G:\code_zhulj\SEIMS\model_data\TEST\model_dianbu_30m_longterm 8 0 127.0.0.1 27017 0`
+ `G:\compile\SEIMS\Release\seims_omp G:\code_zhulj\SEIMS\model_data\TEST\model_dianbu_30m_longterm 8 0 192.168.6.55 27017 0`

+ `G:\compile\SEIMS\Release\seims_omp G:\code_zhulj\SEIMS\model_data\dianbu\model_dianbu_30m_longterm 8 0 127.0.0.1 27017 0`

+ `G:\compile\SEIMS\Release\seims_omp G:\code_zhulj\SEIMS\model_data\dianbu\model_dianbu3_30m_longterm 8 0 192.168.6.55 27017 0`
+ `python G:\code_zhulj\SEIMS\preprocess\main.py -ini G:\code_zhulj\SEIMS\preprocess\dianbu_30m_longterm_omp_zhulj_winserver.ini`
+ `python G:\code_zhulj\SEIMS\preprocess\subbasin_delineation.py -ini G:\code_zhulj\SEIMS\preprocess\dianbu2_30m_longterm_omp_zhulj_winserver.ini`
+ `python G:\code_zhulj\SEIMS\preprocess\import_bmp_scenario.py -ini G:\code_zhulj\SEIMS\preprocess\dianbu2_30m_longterm_omp_zhulj_winserver.ini`

+ `G:\compile\SEIMS_Preprocess\Release\grid_layering 192.168.6.55 27017 G:\data_m\SEIMS_dataprepare\model_dianbu2_30m_longterm\layering_info model_dianbu2_30m_longterm SPATIAL 1`

## TEST COMMAND ON DGPM-cluster:
+ python /home/zhulj/SEIMS/SEIMS/preprocess/main.py /home/zhulj/SEIMS/SEIMS/preprocess/dianbu_30m_longterm_omp_dgpm.ini
+ /home/zhulj/SEIMS/seims_omp/seims_omp /home/zhulj/SEIMS/SEIMS/model_data/dianbu/model_dianbu_30m_longterm 8 0 127.0.0.1 27017 0

Preprocess command:
python G:\code_zhulj\SEIMS\preprocess\main.py -ini G:\code_zhulj\SEIMS\preprocess\dianbu_30m_longterm_omp_zhulj_winserver.ini

python G:\code_zhulj\SEIMS\preprocess\import_parameters.py -ini G:\code_zhulj\SEIMS\preprocess\dianbu_30m_longterm_omp_zhulj_winserver.ini

python G:\code_zhulj\SEIMS\preprocess\import_bmp_scenario.py -ini G:\code_zhulj\SEIMS\preprocess\dianbu_30m_longterm_omp_zhulj_winserver.ini
python G:\code_zhulj\SEIMS\preprocess\parameters_extraction.py -ini G:\code_zhulj\SEIMS\preprocess\dianbu_30m_longterm_omp_zhulj_winserver.ini

## TEST COMMAND BY Huiran Gao:

+ D:\SEIMS_model\SEIMS\Release\seims_omp D:\SEIMS_model\Model_data\model_dianbu_30m_longterm 8 0 192.168.6.55 27017 6
+ D:\SEIMS_model\SEIMS\Release\seims_omp D:\SEIMS_model\Model_data\model_dianbu2_30m_longterm 8 0 192.168.6.55 27017 0

+ python D:\GaohrWS\GithubPrj\SEIMS\preprocess\import_parameters.py -ini D:\GaohrWS\GithubPrj\SEIMS\preprocess\dianbu_30m_longterm_omp_ghr_win.ini
+ python D:\GaohrWS\GithubPrj\SEIMS\preprocess\import_bmp_scenario.py -ini D:\GaohrWS\GithubPrj\SEIMS\preprocess\dianbu_30m_longterm_omp_ghr_win.ini
+ python D:\GaohrWS\GithubPrj\SEIMS\preprocess\main.py -ini D:\GaohrWS\GithubPrj\SEIMS\preprocess\dianbu_30m_longterm_omp_ghr_win.ini

+ D:\SEIMS_model\SEIMS_preprocessing\build\Release\grid_layering 192.168.6.55 27017 D:\SEIMS_model\Model_data\dianbu2_30m_omp\layering_info model_dianbu2_30m_longterm SPATIAL 0

## Under LINUX-Cluster 192.168.6.55

+ The released version is located in /home/zhulj/SEIMS/seims_omp_rel_x86-XXXXXX, e.g., /home/zhulj/SEIMS/seims_omp_rel_x86-201611/seims_omp
  + omp or mpi is the parallel version
  + x86 or x64 is the compiled version (currently, only x86 is provided)
  + python /home/zhulj/SEIMS/seims_omp_rel_x86-201611/preprocess/main.py -ini /home/zhulj/SEIMS/models/dianbu/dianbu2_30m_longterm_omp_dgpm.ini
  + python /home/zhulj/SEIMS/seims_omp_rel_x86-201611/preprocess/gen_subbasins.py -ini /home/zhulj/SEIMS/models/dianbu/dianbu2_30m_longterm_omp_dgpm.ini
+ Commands
  + /home/zhulj/SEIMS/seims_omp/seims_omp /home/zhulj/SEIMS/models/dianbu/model_dianbu2_30m_longterm 6 0 192.168.6.55 27017 0
  + /home/zhulj/SEIMS/seims_omp_rel_x86-201611/seims_omp /home/zhulj/SEIMS/models/dianbu/model_dianbu2_30m_longterm 8 0 192.168.6.55 27017 0
+ Postprocess
  + python /home/zhulj/SEIMS/seims_omp_rel_x86-201611/postprocess/hydroPlot_main.py -ini /home/zhulj/SEIMS/models/dianbu/post_dianbu_30m_longterm_omp_dgpm.ini
  
+ Scenario analysis
  + python -m scoop --hostfile /home/zhulj/SEIMS/models/dianbu/dgpm_hosts_SCOOP -n 8 /home/zhulj/SEIMS/seims_omp_rel_x86-201611/scenario_analysis/nsga2.py -ini /home/zhulj/SEIMS/models/dianbu/nsgaii_dianbu2_30m_longterm_omp_dgpm.ini
  
+ 192.168.6.57
  + python G:\code_zhulj\SEIMS\preprocess\import_parameters.py -ini G:\code_zhulj\SEIMS\preprocess\dianbu2_30m_longterm_omp_zhulj_winserver.ini

+ postprocess
  + D:\GaohrWS\GithubPrj\SEIMS\postprocess>python hydroPlot_main.py -ini post_dianbu_30m_longterm_omp_gaohr_win.ini

+ NSGA-II
  + python D:\GaohrWS\GithubPrj\SEIMS\nsgaii_for_SEIMS\nsga2_deap\nsga2.py -ini D:\GaohrWS\GithubPrj\SEIMS\nsgaii_for_SEIMS\nsga2_deap\nsgaii_dianbu2_30m_longterm_omp_gaohr_win.ini
  + python -m scoop -n 4 D:\GaohrWS\GithubPrj\SEIMS\nsgaii_for_SEIMS\nsga2.py -ini D:\GaohrWS\GithubPrj\SEIMS\nsgaii_for_SEIMS\nsgaii_dianbu2_30m_longterm_omp_gaohr_win.ini


  + python C:\z_code\Hydro\SEIMS\scenario_analysis\nsga2.py -ini C:\z_code\Hydro\SEIMS\scenario_analysis\nsgaii_dianbu2_30m_longterm_omp_zhulj_win.ini
  + python -m scoop -n 4 C:\z_code\Hydro\SEIMS\scenario_analysis\nsga2.py -ini C:\z_code\Hydro\SEIMS\scenario_analysis\nsgaii_dianbu2_30m_longterm_omp_zhulj_win.ini
  
  
+ permission denied: chmod 
