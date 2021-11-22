classdef GetDENMSituationContainerEventHistory < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        EventHistorySize = 1;       
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [EventHistory, ActualEventHistorySize] = stepImpl(obj, StationID, SequenceNumber) 
            if coder.target('Rtw') || coder.target('Sfun') 
                if obj.EventHistorySize > 0
                    LinearHistory = int32(zeros(obj.EventHistorySize * 5, 1));
                    ActualEventHistorySize = int32(0);
                    coder.cinclude('c2xdenm.h');
                    coder.ceval('getDENMSituationContainerEventHistory', StationID, SequenceNumber, coder.ref(LinearHistory), obj.EventHistorySize * 5, coder.ref(ActualEventHistorySize));
                    EventHistory = transpose(reshape(LinearHistory, [5, obj.EventHistorySize]));                   
                end                
            end            
        end
        
        function releaseImpl(~)            
        end        

    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetDENMSituationContainerEventHistory';
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
            libName =  strcat('c2xdenm', linkLibExt);
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
        function [EventHistory, ActualEventHistorySize] = getOutputSizeImpl(obj)
            EventHistory = [obj.EventHistorySize 5];
            ActualEventHistorySize = [1 1];
        end 
        
        function [EventHistory, ActualEventHistorySize] = isOutputFixedSizeImpl(obj)
            EventHistory = true;
            ActualEventHistorySize = true;
        end
        
        function [EventHistory, ActualEventHistorySize] = getOutputDataTypeImpl(obj)
            EventHistory = 'int32';
            ActualEventHistorySize = 'int32';
        end
        
        function [EventHistory, ActualEventHistorySize] = isOutputComplexImpl(obj)
            EventHistory = false;
            ActualEventHistorySize = false;
        end
    end
end