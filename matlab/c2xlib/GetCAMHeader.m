classdef GetCAMHeader < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
          
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [ProtocolVersion, MessageID] = stepImpl(obj, StationID) 
            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                ProtocolVersion = int32(0);
                MessageID = int32(0);
                coder.cinclude(LibConfig.getCAMHeader());
                err = coder.ceval('getCAMHeader', StationID, coder.ref(ProtocolVersion), coder.ref(MessageID));
                LibConfig.printErrorCode(err);
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetCAMHeader';
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
        function [ProtocolVersion, MessageID] = getOutputSizeImpl(obj)
            ProtocolVersion = [1, 1];
            MessageID = [1, 1];
        end 
        
        function [ProtocolVersion, MessageID] = isOutputFixedSizeImpl(obj)
            ProtocolVersion = true;
            MessageID = true;
        end
        
        function [ProtocolVersion, MessageID] = getOutputDataTypeImpl(obj)
            ProtocolVersion = 'int32';
            MessageID = 'int32';
        end
        
        function [ProtocolVersion, MessageID] = isOutputComplexImpl(obj)
            ProtocolVersion = false;
            MessageID = false;
        end
    end
end