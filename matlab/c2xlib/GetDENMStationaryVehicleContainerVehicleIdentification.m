classdef GetDENMStationaryVehicleContainerVehicleIdentification < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        WMInumberSize = int32(16);
        VDSSize = int32(16);
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [WMInumber, VDS] = stepImpl(obj, StationID, SequenceNumber) 
            if coder.target('Rtw') || coder.target('Sfun') 
                WMInumber = uint8(zeros(obj.WMInumberSize, 1));
                VDS = uint8(zeros(obj.VDSSize, 1));
                coder.cinclude(LibConfig.getDENMHeader());
                coder.ceval('getDENMStationaryVehicleContainerVehicleIdentification', StationID, SequenceNumber, ...
                    coder.ref(WMInumber), length(WMInumber), coder.ref(VDS), length(VDS));
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetDENMStationaryVehicleContainerVehicleIdentification';
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
        function [WMInumber, VDS] = getOutputSizeImpl(obj)
            WMInumber = [obj.WMInumberSize 1];
            VDS = [obj.VDSSize 1];
        end 
        
        function [WMInumber, VDS] = isOutputFixedSizeImpl(obj)
            WMInumber = true;
            VDS = true;
        end
        
        function [WMInumber, VDS] = getOutputDataTypeImpl(obj)
            WMInumber = 'uint8';
            VDS = 'uint8';
        end
        
        function [WMInumber, VDS] = isOutputComplexImpl(obj)
            WMInumber = false;
            VDS = false;
        end
    end
end