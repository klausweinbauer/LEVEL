classdef GetDENMRoadWorksContainerExtendedRestriction < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        StationTypesSize = 1;
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [StationTypes] = stepImpl(obj, StationID, SequenceNumber) 
            if coder.target('Rtw') || coder.target('Sfun') 
                StationTypes = int32(zeros(obj.StationTypesSize, 1));
                coder.cinclude(LibConfig.getDENMHeader());
                coder.ceval('getDENMRoadWorksContainerExtendedRestriction', StationID, SequenceNumber, ...
                    coder.ref(StationTypes), length(StationTypes));
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetDENMRoadWorksContainerExtendedRestriction';
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

    methods (Access = protected)
        function [StationTypes] = getOutputSizeImpl(obj)
            StationTypes = [obj.StationTypesSize 1];
        end 
        
        function [StationTypes] = isOutputFixedSizeImpl(obj)
            StationTypes = true;
        end
        
        function [StationTypes] = getOutputDataTypeImpl(obj)
            StationTypes = 'int32';
        end
        
        function [StationTypes] = isOutputComplexImpl(obj)
            StationTypes = false;
        end
    end
end