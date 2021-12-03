classdef GetCAMBasicVehicleContainerLowFrequencyPathHistory < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        PathHistorySize = 1;  
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [PathHistory, ActualPathHistorySize] = stepImpl(obj, StationID) 
            if coder.target('Rtw') || coder.target('Sfun') 
                TmpPathHistory = int32(zeros(obj.PathHistorySize * 4, 1));
                TmpActualPathHistorySize = int32(0);
                coder.cinclude(LibConfig.getCAMHeader());
                coder.ceval('getCAMBasicVehicleContainerLowFrequencyPathHistory', StationID, ...
                    coder.ref(TmpPathHistory), length(TmpPathHistory), coder.ref(TmpActualPathHistorySize));
                PathHistory = transpose(reshape(TmpPathHistory, 4, obj.PathHistorySize));
                ActualPathHistorySize = TmpActualPathHistorySize / 4;
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetCAMBasicVehicleContainerLowFrequencyPathHistory';
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
            LibConfig.updateBuildInfoCAM(buildInfo, buildContext);
        end
    end
    methods (Access = protected)
        function [PathHistory, ActualPathHistorySize] = getOutputSizeImpl(obj)
            PathHistory = [obj.PathHistorySize, 4];
            ActualPathHistorySize = [1, 1];
        end 
        
        function [PathHistory, ActualPathHistorySize] = isOutputFixedSizeImpl(obj)
            PathHistory = true;
            ActualPathHistorySize = true;
        end
        
        function [PathHistory, ActualPathHistorySize] = getOutputDataTypeImpl(obj)
            PathHistory = 'int32';
            ActualPathHistorySize = 'int32';
        end
        
        function [PathHistory, ActualPathHistorySize] = isOutputComplexImpl(obj)
            PathHistory = false;
            ActualPathHistorySize = false;
        end
    end
end