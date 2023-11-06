:: windows bat 多行输入, 比较困难, 建议逐行打印/直接写好文件进行拷贝(换行符号不是'\')
:: 换行必须使用CRLF win专用换行符号, 而不是linux 专用LF
:: 特殊字符'"', 使用"" 而不是\"



echo (":: adan version for build ng-tsp 
set QT_DIR= ^
") > ./baby.bat
