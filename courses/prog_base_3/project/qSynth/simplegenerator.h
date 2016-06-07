#ifndef QSYNTH_SIMPLEGENERATOR_H
#define QSYNTH_SIMPLEGENERATOR_H

#include "guitargenerator.h"
#include "iaudiocallback.h"
#include "igenericinput.h"
#include "spinlock.h"
#include <vector>

namespace qSynth {

struct Wave {
    int frequency;
    unsigned long played_time;
};

class SimpleGenerator : public IAudioCallback
{
public:
    SimpleGenerator();
    void processInput(const std::vector<GenericInputAction>& input) override;
    void fillBuffer(float* buffer, unsigned long frames) override;
private:
    //std::vector<Wave> waves;
    guitar::GuitarGenerator guitar_gen;

    util::Spinlock input_lock;
    std::vector<GenericInputAction> danger_buffer;
    void dangerProcessInput();
};

} // namespace qSynth

#endif // QSYNTH_SIMPLEGENERATOR_H