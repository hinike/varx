#pragma once

/**
 Connects a juce::Value with a BehaviorSubject.
 
 Whenever the Value changes, the BehaviorSubject is changed, and vice versa.
 */
class ValueExtension : public ExtensionBase, private juce::Value::Listener
{
public:
    /** Creates a new instance with a given Value. The connection refers to the **`ValueSource`** of `inputValue`. */
    ValueExtension(const juce::Value& inputValue);
    
    /** The subject that's connected to the Value. This changes whenever the Value changes, and vice versa. */
    const BehaviorSubject subject;
    
private:
    juce::Value value;
    
    void valueChanged(juce::Value&) override;
};

/**
    Adds reactive extensions to a juce::AudioProcessor.
 */
class AudioProcessorExtension : public ExtensionBase, private juce::AudioProcessorListener
{
    PublishSubject _processorChanged;
public:
    /** Creates a new instance for a given AudioProcessor. */
    AudioProcessorExtension(juce::AudioProcessor& parent);

    /** Emits when something (apart from a parameter value) has changed. For example the program, number of parameters, etc.​ **Type: undefined**. */
    const Observable processorChanged;

private:
    void audioProcessorParameterChanged(juce::AudioProcessor*, int, float) override {}
    void audioProcessorChanged(juce::AudioProcessor*) override;
    void audioProcessorParameterChangeGestureBegin(juce::AudioProcessor*, int) override {}
    void audioProcessorParameterChangeGestureEnd(juce::AudioProcessor*, int) override {}
};
