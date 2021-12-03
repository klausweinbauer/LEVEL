classdef DecodeDENM < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [StationID, SequenceNumber] = stepImpl(obj, Buffer, BufferSize)
            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                StationID = int32(0);
                SequenceNumber = int32(0);
                coder.cinclude(LibConfig.getDENMHeader());
                err = coder.ceval('decodeDENM', coder.ref(StationID), coder.ref(SequenceNumber), coder.ref(Buffer), BufferSize);
                LibConfig.printErrorCode(err);
            end            
        end
        
        function releaseImpl(~)            
        end

    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'DecodeDENM';
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