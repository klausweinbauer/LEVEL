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

        function printErrorCode(err)
            if (err < 0)
                MsgBytes = uint8(zeros(255, 1));
                MsgLength = int32(0);
                coder.ceval('getLastErrMsg', coder.ref(MsgBytes), length(MsgBytes), coder.ref(MsgLength));
                type = LibConfig.getErrorName(err);
                Msg = sprintf('[%s] %s', type, char(MsgBytes));
                error(Msg);
            end
        end

        function [name] = getErrorName(err)
            name = '';
            if (err == -1)
                name = 'ERR_MSG_NOT_FOUND';
            elseif (err == -2)
                name = 'ERR_ALLOC_FAILED';
            elseif (err == -3)
                name = 'ERR_NULL';
            elseif (err == -4)
                name = 'ERR_BUFFER_OVERFLOW';
            elseif (err == -5)
                name = 'ERR_ENCODE';
            elseif (err == -6)
                name = 'ERR_DECODE';
            elseif (err == -7)
                name = 'ERR_RECEIVER_START';
            elseif (err == -8)
                name = 'ERR_TRANSMITTER_START';
            elseif (err == -9)
                name = 'ERR_ARG_NULL';
            elseif (err == -10)
                name = 'ERR_INDEX_OUT_OF_RANGE';
            elseif (err == -20)
                name = 'ERR_HIGH_FREQ_CONTAINER_TYPE';
            elseif (err == -21)
                name = 'ERR_LOW_FREQ_CONTAINER_TYPE';
            elseif (err == -22)
                name = 'ERR_CAM_ALREADY_EXISTS';
            elseif (err == -23)
                name = 'ERR_SPECIAL_VEHICLE_CONTAINER_TYPE';
            elseif (err == -40)
                name = 'ERR_DENM_ALREADY_EXISTS';
            end
        end
    end


end