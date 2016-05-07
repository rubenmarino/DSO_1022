
short WINAPI dsoOpenDevice(WORD DeviceIndex);
short WINAPI dsoSetVoltDIV(WORD DeviceIndex, int nCH, int nVoltDIV);
short WINAPI dsoSetTimeDIV(WORD DeviceIndex, int nTimeDIV);
short WINAPI dsoReadHardData(
        WORD DeviceIndex,
        short* pCH1Data,
        short* pCH2Data,
        ULONG nReadLen,
        short* pCalLevel,
        int nCH1VoltDIV,
        int nCH2VoltDIV,
        short nTrigSweep,
        short nTrigSrc,
        short nTrigLevel,
        short nSlope,
        int nTimeDIV,
        short nHTrigPos,
        ULONG nDisLen,
        ULONG* nTrigPoint,
        short nInsertMode);
WORD WINAPI dsoGetCalLevel(WORD DeviceIndex, short* level, short nLen);
short WINAPI dsoCalibrate(WORD nDeviceIndex, int nTimeDIV, int nCH1VoltDIV, int nCH2VoltDIV, short* pCalLevel);
WORD WINAPI dsoSetCalLevel(WORD DeviceIndex, short* level, short nLen);

