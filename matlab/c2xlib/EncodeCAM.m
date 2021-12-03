classdef EncodeCAM < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        BufferSize = int32(65535);
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [Buffer, ActualBufferSize] = stepImpl(obj, StationID) 
            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                Buffer = uint8(zeros(obj.BufferSize, 1));
                ActualBufferSize = int32(0);                
                coder.cinclude('c2xcam.h');
                err = coder.ceval('encodeCAM', StationID, coder.ref(Buffer), obj.BufferSize, coder.ref(ActualBufferSize));
                obj.printErrorCode(err);
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
            bName = 'EncodeCAM';
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
        function [Buffer, ActualBufferSize] = getOutputSizeImpl(obj)
            Buffer = [obj.BufferSize, 1];
            ActualBufferSize = [1, 1];
        end 
        
        function [Buffer, ActualBufferSize] = isOutputFixedSizeImpl(obj)
            Buffer = true;
            ActualBufferSize = true;
        end
        
        function [Buffer, ActualBufferSize] = getOutputDataTypeImpl(obj)
            Buffer = 'uint8';
            ActualBufferSize = 'int32';
        end
        
        function [Buffer, ActualBufferSize] = isOutputComplexImpl(obj)
            Buffer = false;
            ActualBufferSize = false;
        end
    end
end