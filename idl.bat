set OSPLDDS_ROOT = %~dp0spliceForTool\external\OpenSplice\x64

REM 清除先前的数据
REM del /q %~dp0spliceForTool\dds\dds_msg_types\*.h %~dp0spliceForTool\dds\dds_msg_types\*.cpp


%~dp0spliceForTool/external/OpenSplice/x64/bin/idlpp -I "%~dp0spliceForTool/external/OpenSplice/x64/etc/idl" -l cpp -P  OS_EXAMPLE_API,examples_export.h -d "%~dp0spliceForTool/dds/dds_msg_types/" "%~dp0spliceForTool/dds/dds_msg_types/idl/WaitSetData.idl" 
