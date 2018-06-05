##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=bamToFreq
ConfigurationName      :=Debug
WorkspacePath          :=/TL/stat_learn/work/mdoering/codeliteWorkspace
ProjectPath            :=/TL/stat_learn/work/mdoering/codeliteWorkspace/bamToFreq/bamToFreq
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Matthias Doering
Date                   :=05/06/18
CodeLitePath           :=/home/mdoering/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="bamToFreq.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)htslib/include/ 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)hts $(LibrarySwitch)boost_filesystem $(LibrarySwitch)boost_system $(LibrarySwitch)z $(LibrarySwitch)pthread $(LibrarySwitch)lzma $(LibrarySwitch)bz2 $(LibrarySwitch)curl $(LibrarySwitch)dl $(LibrarySwitch)ssl $(LibrarySwitch)crypto 
ArLibs                 :=  "libhts" "boost_filesystem" "boost_system" "z" "pthread" "lzma" "bz2" "curl" "dl" "ssl" "crypto" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)htslib/lib 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -Wall -std=c++11 $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall  $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/bamReader.cpp$(ObjectSuffix) $(IntermediateDirectory)/FreqMatrix.cpp$(ObjectSuffix) $(IntermediateDirectory)/ReadData.cpp$(ObjectSuffix) $(IntermediateDirectory)/CodonData.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/bamReader.cpp$(ObjectSuffix): bamReader.cpp $(IntermediateDirectory)/bamReader.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/TL/stat_learn/work/mdoering/codeliteWorkspace/bamToFreq/bamToFreq/bamReader.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bamReader.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bamReader.cpp$(DependSuffix): bamReader.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bamReader.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bamReader.cpp$(DependSuffix) -MM bamReader.cpp

$(IntermediateDirectory)/bamReader.cpp$(PreprocessSuffix): bamReader.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bamReader.cpp$(PreprocessSuffix) bamReader.cpp

$(IntermediateDirectory)/FreqMatrix.cpp$(ObjectSuffix): FreqMatrix.cpp $(IntermediateDirectory)/FreqMatrix.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/TL/stat_learn/work/mdoering/codeliteWorkspace/bamToFreq/bamToFreq/FreqMatrix.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FreqMatrix.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FreqMatrix.cpp$(DependSuffix): FreqMatrix.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FreqMatrix.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FreqMatrix.cpp$(DependSuffix) -MM FreqMatrix.cpp

$(IntermediateDirectory)/FreqMatrix.cpp$(PreprocessSuffix): FreqMatrix.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FreqMatrix.cpp$(PreprocessSuffix) FreqMatrix.cpp

$(IntermediateDirectory)/ReadData.cpp$(ObjectSuffix): ReadData.cpp $(IntermediateDirectory)/ReadData.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/TL/stat_learn/work/mdoering/codeliteWorkspace/bamToFreq/bamToFreq/ReadData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ReadData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ReadData.cpp$(DependSuffix): ReadData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ReadData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ReadData.cpp$(DependSuffix) -MM ReadData.cpp

$(IntermediateDirectory)/ReadData.cpp$(PreprocessSuffix): ReadData.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ReadData.cpp$(PreprocessSuffix) ReadData.cpp

$(IntermediateDirectory)/CodonData.cpp$(ObjectSuffix): CodonData.cpp $(IntermediateDirectory)/CodonData.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/TL/stat_learn/work/mdoering/codeliteWorkspace/bamToFreq/bamToFreq/CodonData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CodonData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CodonData.cpp$(DependSuffix): CodonData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CodonData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CodonData.cpp$(DependSuffix) -MM CodonData.cpp

$(IntermediateDirectory)/CodonData.cpp$(PreprocessSuffix): CodonData.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CodonData.cpp$(PreprocessSuffix) CodonData.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


