classdef EncodeDENM < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        BufferSize = int32(16384);
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [Buffer, ActualBufferSize] = stepImpl(obj, StationID, SequenceNumber) 
            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                Buffer = uint8(zeros(obj.BufferSize, 1));
                ActualBufferSize = int32(0);                
                coder.cinclude(LibConfig.getDENMHeader());
                err = coder.ceval('encodeDENM', StationID, SequenceNumber, coder.ref(Buffer), obj.BufferSize, coder.ref(ActualBufferSize));
                LibConfig.printErrorCode(err);
            end            
        end
        
        function releaseImpl(~)            
        end

    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'EncodeDENM';
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