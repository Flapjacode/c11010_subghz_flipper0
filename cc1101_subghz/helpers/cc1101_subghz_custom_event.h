#pragma once

typedef enum {
    // Scene events
    CC1101SubGhzCustomEventReserved = 0,
    
    // Start scene events
    CC1101SubGhzCustomEventStartStaticScan,
    CC1101SubGhzCustomEventStartHoppingScan,
    CC1101SubGhzCustomEventStartSavedSignals,
    CC1101SubGhzCustomEventStartSettings,
    
    // Scanning view events
    CC1101SubGhzCustomEventViewScanningOk,
    CC1101SubGhzCustomEventViewScanningBack,
    CC1101SubGhzCustomEventViewScanningLeft,
    CC1101SubGhzCustomEventViewScanningRight,
    CC1101SubGhzCustomEventViewScanningUp,
    CC1101SubGhzCustomEventViewScanningDown,
    
    // Hopping view events
    CC1101SubGhzCustomEventViewHoppingOk,
    CC1101SubGhzCustomEventViewHoppingBack,
    CC1101SubGhzCustomEventViewHoppingLeft,
    CC1101SubGhzCustomEventViewHoppingRight,
    CC1101SubGhzCustomEventViewHoppingUp,
    CC1101SubGhzCustomEventViewHoppingDown,
    
    // Signal events
    CC1101SubGhzCustomEventSignalDetected,
    CC1101SubGhzCustomEventSignalLost,
    CC1101SubGhzCustomEventTransmitStart,
    CC1101SubGhzCustomEventTransmitStop,
    CC1101SubGhzCustomEventTransmitComplete,
    
    // Settings events
    CC1101SubGhzCustomEventSettingsFrequencyChanged,
    CC1101SubGhzCustomEventSettingsModulationChanged,
    CC1101SubGhzCustomEventSettingsBandwidthChanged,
    CC1101SubGhzCustomEventSettingsDataRateChanged,
    CC1101SubGhzCustomEventSettingsRSSIThresholdChanged,
    CC1101SubGhzCustomEventSettingsHopIntervalChanged,
    CC1101SubGhzCustomEventSettingsReset,
    CC1101SubGhzCustomEventSettingsSave,
    
    // Hardware events
    CC1101SubGhzCustomEventCC1101Error,
    CC1101SubGhzCustomEventCC1101Ready,
    CC1101SubGhzCustomEventCC1101ConfigUpdate,
    
    // Timer events
    CC1101SubGhzCustomEventTimerTick,
    CC1101SubGhzCustomEventHopTimer,
    CC1101SubGhzCustomEventScanTimer,
    CC1101SubGhzCustomEventTransmitTimer,
    
    // Worker thread events
    CC1101SubGhzCustomEventWorkerStart,
    CC1101SubGhzCustomEventWorkerStop,
    CC1101SubGhzCustomEventWorkerData,
    CC1101SubGhzCustomEventWorkerError,
    
    // File operations
    CC1101SubGhzCustomEventFileSave,
    CC1101SubGhzCustomEventFileLoad,
    CC1101SubGhzCustomEventFileDelete,
    CC1101SubGhzCustomEventFileError,
    
    // Navigation events
    CC1101SubGhzCustomEventNavBack,
    CC1101SubGhzCustomEventNavNext,
    CC1101SubGhzCustomEventNavSelect,
    CC1101SubGhzCustomEventNavExit,
    
    // Last event marker
    CC1101SubGhzCustomEventMax
} CC1101SubGhzCustomEvent;