classdef AddDENMSituationContainerEventPoint < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
        SampleActive = true;
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [] = stepImpl(obj, StationID, SequenceNumber, SampleTrigger, DeltaLatitude, DeltaLongitude, DeltaAltitude, DeltaTime, InformationQuality) 
            if coder.target('Rtw') || coder.target('Sfun') 
                coder.cinclude(LibConfig.getDENMHeader());
                if obj.SampleActive && SampleTrigger ~= 0
                    obj.SampleActive = false;
                    err = int32(0);
                    err = coder.ceval('addDENMSituationContainerEventPoint', StationID, SequenceNumber, DeltaLatitude, DeltaLongitude, DeltaAltitude, DeltaTime, InformationQuality);
                    LibConfig.printErrorCode(err);
                elseif ~obj.SampleActive && SampleTrigger == 0
                    obj.SampleActive = true;
                end
            end            
        end
        
        function releaseImpl(~)            
        end

    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'AddDENMSituationContainerEventPoint';
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
            LibConfig.updateBuildInfoDENM(buildInfo, buildContext);
        end
    end
end