# Manual

# Introduction

Zones is a free convolution reverb plugin that allows you to download samples of real-world spaces
to use directly in a DAW. These spaces, also known as impulse responses, are recorded and uploaded by users.
Via a process called convolution, these impulse responses can be used to make it sound like your input is
being recorded in that space in real-time. This is perfect for a variety of uses, such as transforming a
dry piano recording to a large concert hall, matching reverb for ADR vocals with on-set recordings in
post-production, or creative purposes with interesting synthesised impulse responses.

# What is a Zone?

A Zone can be described as a collection of recordings of a particular space at a particular time.
It can be composed of one or many different IR recordings with a range of speaker positions, microphone
positions and microphone types.

For example, I could record an IR at a concert hall with three speaker positions on the stage
(left, centre and right). One IR might be a spaced pair of omni mics using the left and right speaker
positions for a true stereo recording. I might then move the mics onto the balcony to create a more
distant sounding pair. Finally, I could place an ambisonic mic in the middle of the hall and use the
centre speaker to make a soundfield recording of the hall.

//diagram of a recording set up and corresponding zone page

This idea can be expanded beyond concerts halls, such as different spots in a cave, rooms in a house
or settings on a hardware reverb unit.

However, there could be a variety of reasons to describe recordings as separate zones. For example, if
someone else recorded the same hall their own way, or if I decided to re-record the same hall on a
different day with new techniques and mic placements. In this way, a zone could be described as an 'event'
where recordings were made, rather than exclusively the geographic location. However, zones are linked
to a specific location or hardware unit or synth in that a single zone isn't a collection of kitchen
recordings or a range of cathedrals.

Ultimately, there isn't a hard rule for zones, and it should be down to the user to share their
recordings in the easiest format for everyone to use!

# Getting Started / Workflow example

## Basic Usage

To get started, add zones onto the effects chain of your chosen channel in the DAW. You'll see that there
are three tabs, *Browse*, *Edit* and *Preferences*. Head to *Browse* and you should find a range of zones to use.
You can browse by selecting a category, searching for a specific term or using a previously downloaded one.
CLicking the play button will select a compatible IR, or alternatively click the eye button to see all the
IR's in that zone and select from there.

Selecting an IR will save the IR to your machine's disk and load it in the plugin. From here you can move
to the *Edit* tab to see the IR as a spectrogram and adjust the parameters (*See Parameter Guide*).

## Workflow Example

Say you had a dry stereo piano recording that you wanted to sound nicer. In the *Browse* tab, search for
the *York Minster* zone and click the eye to see the full details. Here you can see details about the
space and its recording. You can experiment with the different mic placements depending on whether you
want the piano to sound close or far. Before selecting the IR, ensure true stereo is selected using
the ... next to the IR. This will perform more convolution creating a richer and more realistic sounding
reverb and stereo image.

//image of zone page

Next head to the *Edit* tab and set Dry/Wet mix to 100% wet, because the IR will already contain the direct signal.
Using the spectrogram, you can edit parameters to fine-tune the sound of the reverb, such as using the trim
to remove any gap at the start, or reverb time to reduce the tail length.

# Parameter guide

The *Edit* tab allows you to edit any IR you have downloaded to make best use of them in your project.
They can be categorised as *IR controls* and *real-time controls*.

## Ir Controls

IR controls directly edit the IR then load the new IR to be used by the convolution engine. Because this
editing happens offline, these parameters are not automatable and are designed for you to tweak before
recording. However, you will find that for normal length IR's these controls are responsive and can
change the sound quickly.

The *Main* controls change the length of the IR.

Reverb Time: This applies a fade to the end of the IR, where 100% means that the whole signal is unfaded,
and 0% means that the fade is applied across the whole signal.

Resample: This interpolates the signal to speed-up or slow-down the IR. This changes Pitch and Time together,
where the percentage represents the ratio of the resample, i.e a 50% resample with slow the signal and double the IR
length.

The *Time* controls alter the beginning and length of the IR.

Predelay: This adds a gap to the beginning of the IR between 0-500ms. Note that the delay will affect
the whole signal, including the direct signal if it is included in the IR.

Trim: This removes from the beginning and shifts the IR to the start, allowing any unwanted predelay inherent in the
recording to be
taken out. 100% trim will cause the whole IR to be removed.

Attack: This applies a fade to the start of the IR, where 100% means that the fade will be applied
across the whole IR. Note that this is applied after predelay and trim.

## Real Time Controls

Real time controls are automatable and designed for practical use of the plugin in a DAW.

The *IO* controls affect volume

Input: This affects the input gain to the plugin before convolution.

Dry/Wet Mix: This affects the balance of the unconvolved and convolved signal, where 100% means that the
output only contains the convolved output. Note that for IR's containing the direct signal, this
should be set to 100% to avoid unwanted phase/duplication artefacts.

Output: This affects the output gain after convolution.

The *EQ* controls affect the wet signal.

Bass: This applies a low-shelf filter at 1kHz with a Q factor of 1.

Treble: This applied a high-shelf filter at 1.5kHz with a Q factor of 1.

## Listen Controls

The Listen controls allow you to test the IR with basic samples. Note that in some DAWs, the track must
be playing or set to input monitoring to allow the plugin to make sound.

The test sounds can be played as a one-shot or looped, using the drop-down menu to change input sample.

## Visualiser

The Visualiser allows you to see the IR that you are editing, which is useful to get an understanding
of how each parameter is changing the signal.

the 2D/3D switch changes the type of visualiser.
The cog button toggles the visualiser display controls. Here the sensitivity, contrast, scale type and
colour scheme can be altered.

# Uploading Zones to the web

## Basic usage

## Upload requirements

# Uploading User Zones

User zones are a type of zone you can store locally on your computer, allowing you to quickly try out
your personal recordings in the plug-in. They are slightly simplified compared to zones uploaded to
the web, with reduced metadata features.

Whilst this is great for testing out an impulse response or converting your existing library to be
more easily accessed in the DAW, we strongly encourage you to upload recordings to the zones convolution
website to share your work with others!

To get started with user zones, head to the *Preferences* tab and add a file path to the list of user
paths. This is where your zones will be copied to in the correct format. Note that this is a separate
path to where zones are downloaded from the *Browse* tab.

Next, head to *Browse* and select *Create Zone* in the top right corner. If you are unsure of the
difference between an Zone and an IR, please see *What is a Zone* (LINK).Start by adding details about
the zone such as the title, description and user path to use. Then select *Add IR* for each IR recording,
where you can add the title, description, channel format and speaker positions used. Selecting the
speaker positions will display the correct amount of *Choose* buttons, which allow you to select the
file you want to copy. Each audio file must be separate and the correct number of channels.

This is an example of a true stereo IR:

//Use screenshot of form

Title: Hall Close

Description: Recorded using a C414 pair, spaced 2m apart, 3m from the stage.

Channel Format: Stereo

Speaker Positions: LR

Left: users/myrecording/C414_left.wav

Right users/myrecording/C414_right.wav

Once all the IRs are added, select *Create Zone* to add the audio files to the user path directory.
You can see your user zones on the browse page.

Currently, the easiest way to remove or modify a user zone is to select reveal on the user path in the
*Preferences* tab and manually delete the associated folder. Also note that removing a user path will
not delete the User Ir's inside it.

















