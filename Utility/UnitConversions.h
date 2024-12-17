//
// Created by William Scheirey on 12/17/24.
//

#ifndef GROUNDSTATION_2025_UNITCONVERSIONS_H
#define GROUNDSTATION_2025_UNITCONVERSIONS_H


namespace Utility::UnitConversion
{
    static constexpr float MetersToFeet = 3.28084;
    static constexpr float GsToMeters = 9.80665;
    static constexpr float PascalsToPSI = 0.000145038;
    static constexpr float MillibarsToPascals = 100;

    static constexpr float meters2feet(float meters) { return meters*MetersToFeet; }
    static constexpr float feet2meters(float feet) { return feet/MetersToFeet; }

    static constexpr float gs2meters(float gs) { return gs*GsToMeters; }
    static constexpr float gs2feet(float gs) { return gs*GsToMeters*MetersToFeet; }

    static constexpr float pa2psi(float pa) { return pa*PascalsToPSI; }
    static constexpr float psi2pa(float psi) { return psi/PascalsToPSI; }

    static constexpr float mbar2pa(float mbar) { return mbar*MillibarsToPascals; }
    static constexpr float pa2mbar(float pa) { return pa/MillibarsToPascals; }

    static constexpr float mbar2psi(float mbar) { return mbar * MillibarsToPascals * PascalsToPSI; }
    static constexpr float psi2mbar(float pa) { return pa / PascalsToPSI / MillibarsToPascals; }
};



#endif //GROUNDSTATION_2025_UNITCONVERSIONS_H
