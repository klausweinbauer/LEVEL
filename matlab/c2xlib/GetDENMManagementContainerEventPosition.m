classdef GetDENMManagementContainerEventPosition < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [Latitude, Longitude, PosConfEllSemiMajorConf, PosConfEllSemiMinorConf, PosConfEllSemiMajorOri, AltitudeValue, AltitudeConf] ...
                = stepImpl(obj, StationID, SequenceNumber) 
            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                Latitude = int32(0);
                Longitude = int32(0);
                PosConfEllSemiMajorConf = int32(0);
                PosConfEllSemiMinorConf = int32(0);
                PosConfEllSemiMajorOri = int32(0);
                AltitudeValue = int32(0);
                AltitudeConf = int32(0);
                coder.cinclude(LibConfig.getDENMHeader());
                err = coder.ceval('getDENMManagementContainerEventPosition', StationID, SequenceNumber, coder.ref(Latitude), coder.ref(Longitude), ...
                coder.ref(PosConfEllSemiMajorConf), coder.ref(PosConfEllSemiMinorConf), coder.ref(PosConfEllSemiMajorOri), coder.ref(AltitudeValue), coder.ref(AltitudeConf));
                LibConfig.printErrorCode(err);
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetDENMManagementContainerEventPosition';
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