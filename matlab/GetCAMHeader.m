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
                coder.cinclude('c2xcam.h');
                err = coder.ceval('getCAMHeader', StationID, coder.ref(ProtocolVersion), coder.ref(MessageID));
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
            [~, linkLibExt, execLibExt, ~] = buildContext.getStdLibInfo();

            % Parametrize library extension
            libName =  strcat('c2xcam', linkLibExt);
            % Other linking parameters
            libPath = 'C:\Program Files\Polyspace\R2021a\extern\lib\win64\c2x';
            libPriority = '';
            libPreCompiled = true;
            libLinkOnly = true;

            % Linking command
            buildInfo.addLinkObjects(libName,libPath,libPriority,libPreCompiled,libLinkOnly);
            buildInfo.addIncludePaths(libPath);
            buildInfo.addIncludeFiles('c2xcommon.h');
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