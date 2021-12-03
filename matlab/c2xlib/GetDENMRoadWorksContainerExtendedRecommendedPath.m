classdef GetDENMRoadWorksContainerExtendedRecommendedPath < matlab.System & coder.ExternalDependency
    
    properties (Nontunable)
        ReferencePositionsSize = 1;
    end
    
    properties (Hidden)
    end
    
    properties (Access = protected)
    end

    methods (Access = protected)
        function setupImpl(~)
        end
        
        function [ReferencePositions] = stepImpl(obj, StationID, SequenceNumber) 
            if coder.target('Rtw') || coder.target('Sfun') 
                TmpReferencePositions = int32(zeros(obj.ReferencePositionsSize * 7, 1));
                coder.cinclude(LibConfig.getDENMHeader());
                coder.ceval('getDENMRoadWorksContainerExtendedRecommendedPath', StationID, SequenceNumber, ...
                    coder.ref(TmpReferencePositions), length(TmpReferencePositions));
                ReferencePositions = transpose(reshape(TmpReferencePositions, [7 obj.ReferencePositionsSize]));
            end            
        end
        
        function releaseImpl(~)            
        end
    end
    methods (Static)
        function bName = getDescriptiveName(~)
            bName = 'GetDENMRoadWorksContainerExtendedRecommendedPath';
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
        function [ReferencePositions] = getOutputSizeImpl(obj)
            ReferencePositions = [obj.ReferencePositionsSize 7];
        end 
        
        function [ReferencePositions] = isOutputFixedSizeImpl(obj)
            ReferencePositions = true;
        end
        
        function [ReferencePositions] = getOutputDataTypeImpl(obj)
            ReferencePositions = 'int32';
        end
        
        function [ReferencePositions] = isOutputComplexImpl(obj)
            ReferencePositions = false;
        end
    end
end