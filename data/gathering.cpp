// Reference : https://bab2min.tistory.com/642
#include <cstdio>
#include <fluidsynth.h>
#include <memory>
 
using namespace std;
 
 
template<typename _T>
using managed_ptr = unique_ptr<_T, void(*)(_T*)>;
 
int main() 
{
    auto settings = managed_ptr<fluid_settings_t>{ 
            new_fluid_settings(), delete_fluid_settings };

    fluid_settings_setstr(settings.get(), 
            "audio.file.name", "output.wav");
    fluid_settings_setstr(settings.get(), 
            "player.timing-source", "sample");
    fluid_settings_setint(settings.get(), 
            "synth.lock-memory", 0);
 
    auto synth = managed_ptr<fluid_synth_t>{ 
            new_fluid_synth(settings.get()), delete_fluid_synth };

    fluid_synth_sfload(synth.get(), "MuseScore_General.sf3", 1);
 
    auto sequencer = managed_ptr<fluid_sequencer_t>{ 
            new_fluid_sequencer2(0), delete_fluid_sequencer };
    fluid_seq_id_t synthSeqID = fluid_sequencer_register_fluidsynth(
            sequencer.get(), synth.get());
 
    auto renderer = managed_ptr<fluid_file_renderer_t>{ 
            new_fluid_file_renderer(synth.get()), delete_fluid_file_renderer };
 
    for (size_t i = 0; i < 128; ++i)  //128 instruments
    {
        for (size_t j = 0; j < 50; ++j)  //note 38~87
        {
            size_t point = i * 50 * 2000 + j * 2000;

            auto evt = managed_ptr<fluid_event_t>{ new_fluid_event(), delete_fluid_event };
            fluid_event_set_source(evt.get(), -1);
            fluid_event_set_dest(evt.get(), synthSeqID);
            fluid_event_program_change(evt.get(), 0, i);
            fluid_sequencer_send_at(sequencer.get(), evt.get(), 0 + point, true);
 
            evt = managed_ptr<fluid_event_t>{ new_fluid_event(), delete_fluid_event };
            fluid_event_set_source(evt.get(), -1);
            fluid_event_set_dest(evt.get(), synthSeqID);
            fluid_event_note(evt.get(), 0, 38 + j, 127, 1000);
            fluid_sequencer_send_at(sequencer.get(), evt.get(), 1 + point, true);
 
            evt = managed_ptr<fluid_event_t>{ new_fluid_event(), delete_fluid_event };
            fluid_event_set_source(evt.get(), -1);
            fluid_event_set_dest(evt.get(), synthSeqID);
            fluid_event_all_sounds_off(evt.get(), 0);
            fluid_sequencer_send_at(sequencer.get(), evt.get(), 1999 + point, true);
        }
    }
 
    for (size_t j = 0; j < 46; ++j)  //46 drums
    {
        size_t point = 128 * 50 * 2000 + j * 2000;

        auto evt = managed_ptr<fluid_event_t>{ new_fluid_event(), delete_fluid_event };
        fluid_event_set_source(evt.get(), -1);
        fluid_event_set_dest(evt.get(), synthSeqID);
        fluid_event_note(evt.get(), 9, j + 34, 127, 1000);
        fluid_sequencer_send_at(sequencer.get(), evt.get(), 1 + point, true);
 
        evt = managed_ptr<fluid_event_t>{ new_fluid_event(), delete_fluid_event };
        fluid_event_set_source(evt.get(), -1);
        fluid_event_set_dest(evt.get(), synthSeqID);
        fluid_event_all_sounds_off(evt.get(), 9);
        fluid_sequencer_send_at(sequencer.get(), evt.get(), 1999 + point, true);
    }
 
    while (fluid_file_renderer_process_block(renderer.get()) == FLUID_OK)
    {
        if (fluid_sequencer_get_tick(sequencer.get()) > 2000 * (128 * 50 + 46)) break;
    }
    return 0;
}