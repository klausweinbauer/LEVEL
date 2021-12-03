classdef LibConfig

    methods (Static)

        function path = getBasePath(~)
            path = 'C:\Program Files\c2xlib';
        end

        function path = getDLLPath(~)
            path = 'C:\Program Files\c2xlib\lib';
        end

        function path = getIncludePath(~)
            path = 'C:\Program Files\c2xlib\include';
        end

        function path = getCAMHeader(~)
            path = 'C:\Program Files\c2xlib\include\c2xcam.h';
        end

        function updateBuildInfoCAM(buildInfo, buildContext)
            [~, linkLibExt, execLibExt, ~] = buildContext.getStdLibInfo();

            % Parametrize library extension
            libName =  strcat('c2xcam', linkLibExt);
            % Other linking parameters
            
            libPriority = '';
            libPreCompiled = true;
            libLinkOnly = true;

            % Linking command
            libDLLPath = LibConfig.getDLLPath();
            libIncludePath = LibConfig.getIncludePath();
            buildInfo.addLinkObjects(libName,libDLLPath,libPriority,libPreCompiled,libLinkOnly);
            buildInfo.addIncludePaths(libIncludePath);
            buildInfo.addIncludeFiles('c2xcommon.h');
        end

        function updateBuildInfoDENM(buildInfo, buildContext)
            [~, linkLibExt, execLibExt, ~] = buildContext.getStdLibInfo();

            % Parametrize library extension
            libName =  strcat('c2xdenm', linkLibExt);
            % Other linking parameters
            
            libPriority = '';
            libPreCompiled = true;
            libLinkOnly = true;

            % Linking command
            libDLLPath = LibConfig.getDLLPath();
            libIncludePath = LibConfig.getIncludePath();
            buildInfo.addLinkObjects(libName,libDLLPath,libPriority,libPreCompiled,libLinkOnly);
            buildInfo.addIncludePaths(libIncludePath);
            buildInfo.addIncludeFiles('c2xcommon.h');
        end

        function printErrorCode(~, err)
            if (err < 0)
                MsgBytes = uint8(zeros(255, 1));
                MsgLength = int32(0);
                coder.ceval('getLastErrMsg', coder.ref(MsgBytes), length(MsgBytes), coder.ref(MsgLength));
                type = getErrorName(err);
                error(char(MsgBytes));
            end
        end

        function getErrorName(~, err) 
            if (err == -1)
                error('ERR_MSG_NOT_FOUND')
            elseif (err == -2)
                error('ERR_ALLOC_FAILED')
            elseif (err == -3)
                error('ERR_NULL')
            elseif (err == -4)
                error('ERR_BUFFER_OVERFLOW')
            elseif (err == -5)
                error('ERR_ENCODE')
            elseif (err == -6)
                error('ERR_DECODE')
            elseif (err == -7)
                error('ERR_RECEIVER_START')
            elseif (err == -8)
                error('ERR_TRANSMITTER_START')
            elseif (err == -9)
                error('ERR_ARG_NULL')
            elseif (err == -10)
                error('ERR_INDEX_OUT_OF_RANGE')
            elseif (err == -20)
                error('ERR_HIGH_FREQ_CONTAINER_TYPE')
            elseif (err == -21)
                error('ERR_LOW_FREQ_CONTAINER_TYPE')
            elseif (err == -22)
                error('ERR_CAM_ALREADY_EXISTS')
            elseif (err == -23)
                error('ERR_SPECIAL_VEHICLE_CONTAINER_TYPE')
            elseif (err == -40)
                error('ERR_DENM_ALREADY_EXISTS')
            end
        end
    end


end