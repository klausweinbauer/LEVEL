classdef GetCAMRSUContainerHighFrequency < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        ProtectedCommunicationZoneNumber = 1;
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [Zone1, Zone2, Zone3, Zone4, Zone5, Zone6, Zone7, Zone8, Zone9, Zone10] = stepImpl(obj, StationID) 
            if coder.target('Rtw') || coder.target('Sfun') 
                coder.cinclude('c2xcam.h');
                Zone1 = uint8(zeros(7, 1));
                Zone2 = uint8(zeros(7, 1));
                Zone3 = uint8(zeros(7, 1));
                Zone4 = uint8(zeros(7, 1));
                Zone5 = uint8(zeros(7, 1));
                Zone6 = uint8(zeros(7, 1));
                Zone7 = uint8(zeros(7, 1));
                Zone8 = uint8(zeros(7, 1));
                Zone9 = uint8(zeros(7, 1));
                Zone10 = uint8(zeros(7, 1));                
                if obj.ProtectedCommunicationZoneNumber > 0
                    coder.ceval('getCAMRSUContainerHighFrequencyProtectedCommunicationZone', StationID, 0, coder.ref(Zone1(1)), coder.ref(Zone1(2)), coder.ref(Zone1(3)), coder.ref(Zone1(4)), coder.ref(Zone1(5)), coder.ref(Zone1(6)), coder.ref(Zone1(7)));
                end
                if obj.ProtectedCommunicationZoneNumber > 1
                    coder.ceval('getCAMRSUContainerHighFrequencyProtectedCommunicationZone', StationID, 1, coder.ref(Zone2(1)), coder.ref(Zone2(2)), coder.ref(Zone2(3)), coder.ref(Zone2(4)), coder.ref(Zone2(5)), coder.ref(Zone2(6)), coder.ref(Zone2(7)));
                end
                if obj.ProtectedCommunicationZoneNumber > 2
                    coder.ceval('getCAMRSUContainerHighFrequencyProtectedCommunicationZone', StationID, 2, coder.ref(Zone3(1)), coder.ref(Zone3(2)), coder.ref(Zone3(3)), coder.ref(Zone3(4)), coder.ref(Zone3(5)), coder.ref(Zone3(6)), coder.ref(Zone3(7)));
                end
                if obj.ProtectedCommunicationZoneNumber > 3
                    coder.ceval('getCAMRSUContainerHighFrequencyProtectedCommunicationZone', StationID, 3, coder.ref(Zone4(1)), coder.ref(Zone4(2)), coder.ref(Zone4(3)), coder.ref(Zone4(4)), coder.ref(Zone4(5)), coder.ref(Zone4(6)), coder.ref(Zone4(7)));
                end
                if obj.ProtectedCommunicationZoneNumber > 4
                    coder.ceval('getCAMRSUContainerHighFrequencyProtectedCommunicationZone', StationID, 4, coder.ref(Zone5(1)), coder.ref(Zone5(2)), coder.ref(Zone5(3)), coder.ref(Zone5(4)), coder.ref(Zone5(5)), coder.ref(Zone5(6)), coder.ref(Zone5(7)));
                end
                if obj.ProtectedCommunicationZoneNumber > 5
                    coder.ceval('getCAMRSUContainerHighFrequencyProtectedCommunicationZone', StationID, 5, coder.ref(Zone6(1)), coder.ref(Zone6(2)), coder.ref(Zone6(3)), coder.ref(Zone6(4)), coder.ref(Zone6(5)), coder.ref(Zone6(6)), coder.ref(Zone6(7)));
                end
                if obj.ProtectedCommunicationZoneNumber > 6
                    coder.ceval('getCAMRSUContainerHighFrequencyProtectedCommunicationZone', StationID, 6, coder.ref(Zone7(1)), coder.ref(Zone7(2)), coder.ref(Zone7(3)), coder.ref(Zone7(4)), coder.ref(Zone7(5)), coder.ref(Zone7(6)), coder.ref(Zone7(7)));
                end
                if obj.ProtectedCommunicationZoneNumber > 7
                    coder.ceval('getCAMRSUContainerHighFrequencyProtectedCommunicationZone', StationID, 7, coder.ref(Zone8(1)), coder.ref(Zone8(2)), coder.ref(Zone8(3)), coder.ref(Zone8(4)), coder.ref(Zone8(5)), coder.ref(Zone8(6)), coder.ref(Zone8(7)));
                end
                if obj.ProtectedCommunicationZoneNumber > 8
                    coder.ceval('getCAMRSUContainerHighFrequencyProtectedCommunicationZone', StationID, 8, coder.ref(Zone9(1)), coder.ref(Zone9(2)), coder.ref(Zone9(3)), coder.ref(Zone9(4)), coder.ref(Zone9(5)), coder.ref(Zone9(6)), coder.ref(Zone9(7)));
                end
                if obj.ProtectedCommunicationZoneNumber > 9
                    coder.ceval('getCAMRSUContainerHighFrequencyProtectedCommunicationZone', StationID, 9, coder.ref(Zone10(1)), coder.ref(Zone10(2)), coder.ref(Zone10(3)), coder.ref(Zone10(4)), coder.ref(Zone10(5)), coder.ref(Zone10(6)), coder.ref(Zone10(7)));
                end                
                obj.printErrorCode(err);
            end            
        end
        
        function releaseImpl(~)            
        end

        function numIn = getNumOutputsImpl(obj)
            numIn = obj.ProtectedCommunicationZoneNumber;
        end

    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetCAMRSUContainerHighFrequency';
        end
        
        function supported = isSupportedContext(buildContext)
            myTarget = {'rtw'};
            if  buildContext.isCodeGenTarget(myTarget)
                supported = true;
            else
                error('API only supported for mex, lib, exe, dll');
            end
        end
        
        function updateBuildInfo(buildInfo, buildContext)
            [~, linkLibExt, execLibExt, ~] = buildContext.getStdLibInfo();

            % Parametrize library extension
            libName =  strcat('c2xcam', linkLibExt);
            % Other linking parameters
            libPath = 'C:\Program Files\Polyspace\R2021a\extern\lib\win64\c2x';
            libPriority = '';
            libPreCompiled = true;
            libLinkOnly = true;

            % Linking command
            buildInfo.addLinkObjects(libName,libPath,libPriority,libPreCompiled,libLinkOnly);
            buildInfo.addIncludePaths(libPath);
            buildInfo.addIncludeFiles('c2xcommon.h');
        end
    end

    methods (Access = protected)
        function [Zone1, Zone2, Zone3, Zone4, Zone5, Zone6, Zone7, Zone8, Zone9, Zone10] = getOutputSizeImpl(obj)
            Zone1 = [7 1];
            Zone2 = [7 1];
            Zone3 = [7 1];
            Zone4 = [7 1];
            Zone5 = [7 1];
            Zone6 = [7 1];
            Zone7 = [7 1];
            Zone8 = [7 1];
            Zone9 = [7 1];
            Zone10 = [7 1];
        end 
        
        function [Zone1, Zone2, Zone3, Zone4, Zone5, Zone6, Zone7, Zone8, Zone9, Zone10] = isOutputFixedSizeImpl(obj)
            Zone1 = true;
            Zone2 = true;
            Zone3 = true;
            Zone4 = true;
            Zone5 = true;
            Zone6 = true;
            Zone7 = true;
            Zone8 = true;
            Zone9 = true;
            Zone10 = true;
        end
        
        function [Zone1, Zone2, Zone3, Zone4, Zone5, Zone6, Zone7, Zone8, Zone9, Zone10] = getOutputDataTypeImpl(obj)
            Zone1 = 'int32';
            Zone2 = 'int32';
            Zone3 = 'int32';
            Zone4 = 'int32';
            Zone5 = 'int32';
            Zone6 = 'int32';
            Zone7 = 'int32';
            Zone8 = 'int32';
            Zone9 = 'int32';
            Zone10 = 'int32';
            
        end
        
        function [Zone1, Zone2, Zone3, Zone4, Zone5, Zone6, Zone7, Zone8, Zone9, Zone10] = isOutputComplexImpl(obj)
            Zone1 = false;
            Zone2 = false;
            Zone3 = false;
            Zone4 = false;
            Zone5 = false;
            Zone6 = false;
            Zone7 = false;
            Zone8 = false;
            Zone9 = false;
            Zone10 = false;
        end
    end
end