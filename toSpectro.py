# Reference : https://bab2min.tistory.com/642
import librosa
import librosa.display
import numpy as np
import matplotlib.pyplot as plt
import itertools
 
spt = []
ins = []
n = 0
for instrument, note in itertools.product(range(128), range(50)):
    y, sr = librosa.load('./Audio_Classification/output.wav', sr=None, offset=n, duration=2.0) # from n to n+2
    n += 2
    
    # adding white noise for data argumentation (0,1e-4,1e-3).
    for r in (0, 1e-4, 1e-3):
        ret = librosa.cqt(y + ((np.random.rand(*y.shape) - 0.5) * r if r else 0), sr, 
            hop_length=1024, n_bins=24*7, bins_per_octave=24)
        
        ret = np.abs(ret)
        spt.append(ret) # save spectrogram as a numpy list
        ins.append((instrument, 38 + note)) # save instrument's number and note (labeling)

for note in range(46):
    y, sr = librosa.load('./Audio_Classification/output.wav', sr=None, offset=n, duration=2.0)
    n += 2
    
    #adding white noise for data argumentation (0,1e-5,1e-4,1e-3)*range(7)
    #more arguments for drum sounds because they don't have notes, so have less number of data.
    for r, s in itertools.product([0, 1e-5, 1e-4, 1e-3], range(7)):
        ret = librosa.cqt(y + ((np.random.rand(*y.shape) - 0.5) * r * s if r else 0), sr, 
            hop_length=1024, n_bins=24 * 7, bins_per_octave=24)
        ret = np.abs(ret)
        spt.append(ret)
        ins.append((note + 128, 0))
 
    #below is to display spectrogram
    #librosa.display.specshow(librosa.amplitude_to_db(np.abs(ret), ref=np.max), sr=sr, x_axis='time', y_axis='cqt_note')
    #plt.colorbar(format='%+2.0f dB')
    #plt.title('Constant-Q power spectrum')
    #plt.tight_layout()
    #plt.show()
 
spt = np.array(spt, np.float32)
ins = np.array(ins, np.int16)
np.savez('cqt.npz', spec=spt, instr=ins)

