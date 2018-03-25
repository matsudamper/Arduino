// When
const float when_value = 111;
#define when(value) for (float _when_value = value, cnt = 0; cnt < 1; cnt++)
#define in(start, end) for (float when_value = _when_value, cnt = 0; (start <= when_value && when_value <= end) && cnt < 1; cnt++)
#define until(start, end) for (float when_value = _when_value, cnt = 0; (start <= when_value && when_value < end) && cnt < 1; cnt++)
#define more(value) for (float when_value = _when_value, cnt = 0; (value <= when_value) && cnt < 1; cnt++)