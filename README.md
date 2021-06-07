# AI Instrument Classifier Using Pytorch

Report About this Project is available on "./Final_Report.pdf"   
Presentation Video (Korean) is available on: https://youtu.be/SCpedCQ5zlI    
        
## Data       
1. Download Virtual instruments data from [MuseScore](https://github.com/musescore/MuseScore/raw/2.1/share/sound/FluidR3Mono_GM.sf3)    
2. Run "./data/gathering.cpp" => "output.wav" is generated      
3. Run "./data/toSpectro.py" => "ctq.nqz" is generated
       
or, you can simply download mine [ctq.npz](https://drive.google.com/file/d/1zAsxSKeloyiXsQInsLXToPCVWM8Z516p/view?usp=sharing)       
       
## Model    
1. It is in "./Instrument_Classifier.ipynb" includes training section.      

"./model.pth" has model weights data that made 70% ACC.        
