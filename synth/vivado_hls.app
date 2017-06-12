<project xmlns="com.autoesl.autopilot.project" name="synth" top="HeaderAnalysisTop">
    <includePaths/>
    <libraryPaths/>
    <Simulation>
        <SimFlow name="csim" clean="true" csimMode="0" lastCsimMode="0"/>
    </Simulation>
    <files xmlns="">
        <file name="../../libIP/src/top_tb.cpp" sc="0" tb="1" cflags=" -std=c++0x"/>
        <file name="libIP/src/tstFIFO.hpp" sc="0" tb="false" cflags="-std=c++0x"/>
        <file name="libIP/src/tstFIFO.cpp" sc="0" tb="false" cflags="-std=c++0x"/>
        <file name="libIP/src/pktBasics.hpp" sc="0" tb="false" cflags="-std=c++0x"/>
        <file name="libIP/src/parser_header_template.hpp" sc="0" tb="false" cflags="-std=c++0x"/>
        <file name="libIP/src/defined_types.h" sc="0" tb="false" cflags="-std=c++0x"/>
        <file name="libIP/src/PktFIFO.hpp" sc="0" tb="false" cflags="-std=c++0x"/>
        <file name="libIP/src/MemoryInfer.hpp" sc="0" tb="false" cflags="-std=c++0x"/>
        <file name="libIP/src/Header.hpp" sc="0" tb="false" cflags="-std=c++0x"/>
    </files>
    <solutions xmlns="">
        <solution name="Virtex7" status="active"/>
        <solution name="Zynq" status="inactive"/>
    </solutions>
</project>

