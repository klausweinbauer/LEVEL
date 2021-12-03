classdef GetCAMSpecialTransportContainer < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        LightBarSirenInUseSize = 1;
        SpecialTransportTypeSize = 1;
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [SpecialTransportType, LightBarSirenInUse] = stepImpl(obj, StationID) 
            if coder.target('Rtw') || coder.target('Sfun') 
                SpecialTransportType = uint8(zeros(obj.SpecialTransportTypeSize, 1));
                LightBarSirenInUse = uint8(zeros(obj.LightBarSirenInUseSize, 1));
                coder.cinclude('c2xcam.h');
                coder.ceval('getCAMSpecialTransportContainer', StationID, coder.ref(SpecialTransportType), ...
                    length(SpecialTransportType), coder.ref(LightBarSirenInUse), length(LightBarSirenInUse));
            end
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetCAMSpecialTransportContainer';
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
        function [SpecialTransportType, LightBarSirenInUse] = getOutputSizeImpl(obj)
            SpecialTransportType = [obj.SpecialTransportTypeSize 1];
            LightBarSirenInUse = [obj.LightBarSirenInUseSize 1];
        end 
        
        function [SpecialTransportType, LightBarSirenInUse] = isOutputFixedSizeImpl(obj)
            SpecialTransportType = true;
            LightBarSirenInUse = true;
        end
        
        function [SpecialTransportType, LightBarSirenInUse] = getOutputDataTypeImpl(obj)
            SpecialTransportType = 'uint8'; 
            LightBarSirenInUse = 'uint8'; 
        end
        
        function [SpecialTransportType, LightBarSirenInUse] = isOutputComplexImpl(obj)
            SpecialTransportType = false;
            LightBarSirenInUse = false;
        end
    end
end