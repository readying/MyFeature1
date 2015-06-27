/*F***************************************************************************
 * openSMILE - the Munich open source Multimedia Interpretation by Large-scale
 * Extraction toolkit
 * 
 * (c) 2008-2013, Florian Eyben, Felix Weninger, Florian Gross, Martin
 * Woellmer, Bjoern Schuller
 * 
 * Technische Universität München (TUM), 80333 Munich, Germany
 * 
 * openSMILE is distributed under a research only license. 
 * 
 * You may use, copy, reproduce, and distribute this Software for any
 * non-commercial purpose. Some purposes which can be non-commercial are
 * teaching, academic research, public demonstrations and personal
 * experimentation. You may not use or distribute this Software or any
 * derivative works in any form for commercial purposes. 
 * 
 * See LICENSE.txt in the top level source directory for details.
 * 
 * THIS SOFTWARE COMES "AS IS", WITH NO WARRANTIES. THIS MEANS NO EXPRESS,
 * IMPLIED OR STATUTORY WARRANTY, INCLUDING WITHOUT LIMITATION, WARRANTIES OF
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, ANY WARRANTY AGAINST
 * INTERFERENCE WITH YOUR ENJOYMENT OF THE SOFTWARE OR ANY WARRANTY OF TITLE
 * OR NON-INFRINGEMENT. THERE IS NO WARRANTY THAT THIS SOFTWARE WILL FULFILL
 * ANY OF YOUR PARTICULAR PURPOSES OR NEEDS. ALSO, YOU MUST PASS THIS
 * DISCLAIMER ON WHENEVER YOU DISTRIBUTE THE SOFTWARE OR DERIVATIVE WORKS.
 * NEITHER TUM NOR ANY CONTRIBUTOR TO THE SOFTWARE WILL BE LIABLE FOR ANY
 * DAMAGES RELATED TO THE SOFTWARE OR THIS LICENSE AGREEMENT, INCLUDING
 * DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL OR INCIDENTAL DAMAGES, TO THE
 * MAXIMUM EXTENT THE LAW PERMITS, NO MATTER WHAT LEGAL THEORY IT IS BASED ON.
 * ALSO, YOU MUST PASS THIS LIMITATION OF LIABILITY ON WHENEVER YOU DISTRIBUTE
 * THE SOFTWARE OR DERIVATIVE WORKS.
 ***************************************************************************E*/


// basic functionality / SMILE API
#include <core/dataMemory.hpp>
#include <core/dataReader.hpp>
#include <core/dataWriter.hpp>
#include <core/dataSource.hpp>
#include <core/dataSink.hpp>
#include <core/dataProcessor.hpp>
#include <core/dataSelector.hpp>
#include <core/vectorProcessor.hpp>
#include <core/vectorTransform.hpp>
#include <core/windowProcessor.hpp>
#include <core/winToVecProcessor.hpp>
#include <core/vecToWinProcessor.hpp>

// examples:
#include <examples/exampleSource.hpp>
#include <examples/exampleSink.hpp>

// basic operations:
#include <other/vectorConcat.hpp>
#include <dspcore/windower.hpp>
#include <dspcore/framer.hpp>
#include <other/vectorOperation.hpp>
#include <other/valbasedSelector.hpp>
#include <other/maxIndex.hpp>
#include <dspcore/fullinputMean.hpp>
#include <dspcore/fullturnMean.hpp>

// sources:
#include <iocore/waveSource.hpp>
#include <iocore/arffSource.hpp>
#include <iocore/csvSource.hpp>
#include <iocore/htkSource.hpp>

// sinks:
#include <core/nullSink.hpp>
#include <iocore/csvSink.hpp>
#include <iocore/datadumpSink.hpp>
#include <iocore/arffSink.hpp>
#include <iocore/htkSink.hpp>
#include <iocore/waveSink.hpp>
#include <iocore/waveSinkCut.hpp>

//"fake" sources / producers:
#include <other/bowProducer.hpp>
#include <dsp/signalGenerator.hpp>

// portaudio code:
#ifdef HAVE_PORTAUDIO
 #include <portaudio/portaudioWavplayer.hpp>
 #include <portaudio/portaudioSource.hpp>
 #include <portaudio/portaudioSink.hpp>
 #include <portaudio/portaudioDuplex.hpp>
#endif

// live sinks (classifiers):
#ifdef BUILD_LIBSVM
 #include <classifiers/libsvmliveSink.hpp>
#endif

#ifdef BUILD_RNN
 #include <rnn/rnnSink.hpp>
 #include <rnn/rnnProcessor.hpp>
 #include <rnn/rnnVad2.hpp>
#endif

#ifdef HAVE_JULIUSLIB
#include <classifiers/julius/juliusSink.hpp>
#endif

// dsp core:
#include <dspcore/monoMixdown.hpp>
#include <dspcore/transformFft.hpp>
#include <dspcore/fftmagphase.hpp>
#include <dspcore/amdf.hpp>
#include <dspcore/acf.hpp>
#include <dspcore/preemphasis.hpp>
#include <dspcore/vectorPreemphasis.hpp>
#include <dspcore/vectorMVN.hpp>
#include <dspcore/turnDetector.hpp>
#include <dspcore/deltaRegression.hpp>
#include <dspcore/contourSmoother.hpp>

// dsp advanced:
#include <dsp/smileResample.hpp>
#include <dsp/specResample.hpp>
#include <dsp/dbA.hpp>
#include <dsp/vadV1.hpp>
#include <dsp/specScale.hpp>

// LLD core:
#include <lldcore/mzcr.hpp>
#include <lldcore/energy.hpp>
#include <lldcore/intensity.hpp>
#include <lldcore/melspec.hpp>
#include <lldcore/mfcc.hpp>
#include <lldcore/plp.hpp>
#include <lldcore/spectral.hpp>
#include <lldcore/pitchBase.hpp>
#include <lldcore/pitchACF.hpp>
#include <lldcore/pitchSmoother.hpp>

// LLD (low-level descriptors):
#include <lld/tonespec.hpp>
#include <lld/tonefilt.hpp>
#include <lld/chroma.hpp>
#include <lld/cens.hpp>
#include <lld/pitchSmootherViterbi.hpp>
#include <lld/pitchJitter.hpp>
#include <lld/pitchDirection.hpp>
#include <lld/pitchShs.hpp>
#include <lld/lpc.hpp>
#include <lld/lsp.hpp>
#include <lld/formantLpc.hpp>
#include <lld/formantSmoother.hpp>

// functionals:
#include <functionals/functionals.hpp>
#include <functionals/functionalExtremes.hpp>
#include <functionals/functionalMeans.hpp>
#include <functionals/functionalPeaks.hpp>
#include <functionals/functionalPeaks2.hpp>
#include <functionals/functionalSegments.hpp>
#include <functionals/functionalOnset.hpp>
#include <functionals/functionalMoments.hpp>
#include <functionals/functionalCrossings.hpp>
#include <functionals/functionalPercentiles.hpp>
#include <functionals/functionalRegression.hpp>
#include <functionals/functionalSamples.hpp>
#include <functionals/functionalTimes.hpp>
#include <functionals/functionalDCT.hpp>
#include <functionals/functionalLpc.hpp>

// advanced io
#include <io/libsvmSink.hpp>

// OpenCV:
#ifdef HAVE_OPENCV
  #include <video/openCVSource.hpp>
#endif


DLLEXPORT const registerFunction componentlist[] = {
  // basic functionality / SMILE API
  cDataMemory::registerComponent,
  cDataWriter::registerComponent,
  cDataReader::registerComponent,
  cDataSource::registerComponent,
  cDataSink::registerComponent,
  cDataProcessor::registerComponent,
  cDataSelector::registerComponent,
  cVectorProcessor::registerComponent,
  cVectorTransform::registerComponent,
  cWindowProcessor::registerComponent,
  cWinToVecProcessor::registerComponent,
  cVecToWinProcessor::registerComponent,

  // examples
  cExampleSource::registerComponent,
  cExampleSink::registerComponent,

  // basic operations:
  cVectorConcat::registerComponent,
  cFramer::registerComponent,
  cWindower::registerComponent,
  cVectorOperation::registerComponent,
  cValbasedSelector::registerComponent,
  cMaxIndex::registerComponent,
  cFullinputMean::registerComponent,
  cFullturnMean::registerComponent,

  // sources:
  cWaveSource::registerComponent,
  cArffSource::registerComponent,
  cCsvSource::registerComponent,
  cHtkSource::registerComponent,

  // sinks:
  cNullSink::registerComponent,
  cCsvSink::registerComponent,
  cDatadumpSink::registerComponent,
  cArffSink::registerComponent,
  cHtkSink::registerComponent,
  cWaveSink::registerComponent,
  cWaveSinkCut::registerComponent,

  //"fake" sources / producers:
  cBowProducer::registerComponent,
  cSignalGenerator::registerComponent,

  #ifdef HAVE_PORTAUDIO
   cPortaudioWavplayer::registerComponent,
   cPortaudioSource::registerComponent,
   cPortaudioSink::registerComponent,
   cPortaudioDuplex::registerComponent,
  #endif

  // live sinks (classifiers):
  #ifdef BUILD_LIBSVM
   cLibsvmLiveSink::registerComponent,
  #endif

  #ifdef BUILD_RNN
   cRnnSink::registerComponent,
   cRnnProcessor::registerComponent,
   cRnnVad2::registerComponent,
  #endif

  #ifdef HAVE_JULIUSLIB
   cJuliusSink::registerComponent,
  #endif

  // dsp core:
  cMonoMixdown::registerComponent,
  cTransformFFT::registerComponent,
  cFFTmagphase::registerComponent,
  cAmdf::registerComponent,
  cAcf::registerComponent,
  cPreemphasis::registerComponent,
  cVectorPreemphasis::registerComponent,
  cVectorMVN::registerComponent,
  cTurnDetector::registerComponent,
  cDeltaRegression::registerComponent,
  cContourSmoother::registerComponent,

  // dsp advanced:
  cSmileResample::registerComponent,
  cSpecResample::registerComponent,
  cDbA::registerComponent,
  cVadV1::registerComponent,
  cSpecScale::registerComponent,

  // LLD core:
  cMZcr::registerComponent,
  cEnergy::registerComponent,
  cIntensity::registerComponent,
  cMelspec::registerComponent,
  cMfcc::registerComponent,
  cPlp::registerComponent,
  cSpectral::registerComponent,
  cPitchBase::registerComponent,
  cPitchACF::registerComponent,
  cPitchSmoother::registerComponent,

  // LLD advanced:
  cTonespec::registerComponent,
  cTonefilt::registerComponent,
  cChroma::registerComponent,
  cCens::registerComponent,
  cPitchSmootherViterbi::registerComponent,
  cPitchJitter::registerComponent,
  cPitchDirection::registerComponent,
  cPitchShs::registerComponent,
  cLpc::registerComponent,
  cLsp::registerComponent,
  cFormantLpc::registerComponent,
  cFormantSmoother::registerComponent,

  // functionals:
  cFunctionals::registerComponent,
  cFunctionalExtremes::registerComponent,
  cFunctionalMeans::registerComponent,
  cFunctionalPeaks::registerComponent,
  cFunctionalPeaks2::registerComponent,
  cFunctionalSegments::registerComponent,
  cFunctionalOnset::registerComponent,
  cFunctionalMoments::registerComponent,
  cFunctionalCrossings::registerComponent,
  cFunctionalPercentiles::registerComponent,
  cFunctionalRegression::registerComponent,
  cFunctionalSamples::registerComponent,
  cFunctionalTimes::registerComponent,
  cFunctionalDCT::registerComponent,
  cFunctionalLpc::registerComponent,

  // io advanced:
  cLibsvmSink::registerComponent,
  
  #ifdef HAVE_OPENCV
    cOpenCVSource::registerComponent,
  #endif

  NULL   // the last element must always be NULL !
};
