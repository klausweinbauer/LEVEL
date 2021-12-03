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
                coder.cinclude('c2xdenm.h');
                coder.ceval('getDENMStationaryVehicleContainerVehicleIdentification', StationID, SequenceNumber, ...
                    coder.ref(WMInumber), length(WMInumber), coder.ref(VDS), length(VDS));
            end            
        end
        
        function releaseImpl(~)            
        end

        function printErrorCode(~, err)
            if (err == -22)
                error('ERR_CAM_ALREADY_EXISTS')
            elseif (err == -21)
                error('ERR_LOW_FREQ_CONTAINER_TYPE_BASIC_VEHICLE')
            elseif (err == -20)
                error('ERR_HIGH_FREQ_CONTAINER_TYPE_BASIC_VEHICLE')
            elseif (err == -40)
                error('ERR_DENM_ALREADY_EXISTS')
            elseif (err == -1)
                error('ERR_MSG_NOT_FOUND')
            elseif (err == -9)
                error('ERR_ARG_NULL')
            elseif (err == -8)
                error('ERR_TRANSMITTER_START')
            elseif (err == -7)
                error('ERR_RECEIVER_START')
            elseif (err == -6)
                error('ERR_DECODE')
            elseif (err == -5)
                error('ERR_ENCODE')
            elseif (err == -4)
                error('ERR_BUFFER_OVERFLOW')
            elseif (err == -3)
                error('ERR_NULL')
            elseif (err == -2)
                error('ERR_ALLOC_FAILED')
            end
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