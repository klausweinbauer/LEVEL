classdef DENMMessage < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        StationID = int32(1);
        SequenceNumber = int32(1);  
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(obj)
            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                coder.cinclude(LibConfig.getDENMHeader());
                err = coder.ceval('createDENM', obj.StationID, obj.SequenceNumber);
                LibConfig.printErrorCode(err);
            end 
        end
        
        function [StationID, SequenceNumber] = stepImpl(obj)
            StationID = obj.StationID;   
            SequenceNumber = obj.SequenceNumber;
        end
        
        function releaseImpl(obj)
            if coder.target('Rtw') || coder.target('Sfun') 
                err = int32(0);
                coder.cinclude(LibConfig.getDENMHeader());
                err = coder.ceval('deleteDENM', obj.StationID, obj.SequenceNumber);
                LibConfig.printErrorCode(err);
            end    
        end

    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'CAMMessage';
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