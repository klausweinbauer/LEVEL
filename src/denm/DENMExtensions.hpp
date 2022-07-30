#pragma once

#include <DENM.h>
#include <DENMWrapper.hpp>
#include <DataExtensionMethods.hpp>

namespace level::denm {

// DENM containers
DEF_GET_PTR_PROPERTY(SituationContainer, DENMWrapper, denm.situation, Situation,
                     (*obj))
DEF_REF(SituationContainer, DENM, denm.situation, Situation)

DEF_GET_PTR_PROPERTY(LocationContainer, DENMWrapper, denm.location, Location,
                     (*obj))
DEF_REF(LocationContainer, DENM, denm.location, Location)

DEF_GET_PTR_PROPERTY(AlacarteContainer, DENMWrapper, denm.alacarte, Alacarte,
                     (*obj))
DEF_REF(AlacarteContainer, DENM, denm.alacarte, Alacarte)

// Situation container properties
DEF_PROP(CauseCode_t, SituationContainer, eventType, EventType);
DEF_PROP(InformationQuality_t, SituationContainer, informationQuality,
         InfromationQuality);
DEF_REF(CauseCode, SituationContainer, linkedCause, LinkedCause);
DEF_REF(EventHistory, SituationContainer, eventHistory, EventHistory);

// Location container properties
DEF_PROP(Traces_t, LocationContainer, traces, Traces);
DEF_REFT(RoadType, LocationContainer, roadType, RoadType);
DEF_REF(Speed, LocationContainer, eventSpeed, EventSpeed);
DEF_REF(Heading, LocationContainer, eventPositionHeading, EventHeading);

// Alacarte container properties
DEF_REFT(LanePosition, AlacarteContainer, lanePosition, LanePosition);
DEF_REF(ImpactReductionContainer, AlacarteContainer, impactReduction,
        ImpactReduction);
DEF_REFT(Temperature, AlacarteContainer, externalTemperature, Temperature);
DEF_REF(RoadWorksContainerExtended, AlacarteContainer, roadWorks,
        RoadWorksExtended);
DEF_REFT(PositioningSolutionType, AlacarteContainer, positioningSolution,
         PositioningSolution);
DEF_REF(StationaryVehicleContainer, AlacarteContainer, stationaryVehicle,
        StationaryVehicle);

// Road works container extended properties
DEF_REFT(LightBarSirenInUse, RoadWorksContainerExtended, lightBarSirenInUse,
         LightBarSirenInUse);
DEF_REF(ClosedLanes, RoadWorksContainerExtended, closedLanes, ClosedLanes);
DEF_REF(RestrictedTypes, RoadWorksContainerExtended, restriction,
        RestrictedTypes);
DEF_REFT(SpeedLimit, RoadWorksContainerExtended, speedLimit, SpeedLimit);
DEF_REF(CauseCode, RoadWorksContainerExtended, incidentIndication, CauseCode);
DEF_REF(ItineraryPath, RoadWorksContainerExtended, recommendedPath,
        ItineraryPath);
DEF_REF(DeltaReferencePosition, RoadWorksContainerExtended,
        startingPointSpeedLimit, DeltaReferencePosition);
DEF_REFT(TrafficRule, RoadWorksContainerExtended, trafficFlowRule, TrafficRule);
DEF_REF(ReferenceDenms, RoadWorksContainerExtended, referenceDenms,
        ReferenceDenms);

// Stationary vehicle container properties
DEF_REFT(StationarySince, StationaryVehicleContainer, stationarySince,
         StationarySince);
DEF_REF(CauseCode, StationaryVehicleContainer, stationaryCause, CauseCode);
DEF_REF(DangerousGoodsExtended, StationaryVehicleContainer,
        carryingDangerousGoods, DangerousGoods);
DEF_REFT(NumberOfOccupants, StationaryVehicleContainer, numberOfOccupants,
         NumberOfOccupants);
DEF_REF(VehicleIdentification, StationaryVehicleContainer,
        vehicleIdentification, VehicleIdentification);
DEF_REFT(EnergyStorageType, StationaryVehicleContainer, energyStorageType,
         EnergyStorageType);

} // namespace level::denm