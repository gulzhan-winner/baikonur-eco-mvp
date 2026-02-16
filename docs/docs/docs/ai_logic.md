# AI Logic Description

This MVP uses a rule-based artificial intelligence model
to evaluate environmental risk conditions.

## Input Parameters

The AI logic analyzes data from sensors:

- Temperature (DHT11)
- Humidity (DHT11)
- Motion detection (SR505)
- Sound level (HSD)
- Rain detection (HYDZ)
- Dust container fill level (HC-SR04)

## Risk Evaluation Model

Each parameter contributes to an environmental risk score.

Example logic:

High temperature → increases risk  
Low humidity → increases risk  
High sound level → increases risk  
Motion detected → increases risk  
Rain detected → decreases risk  
Dust container nearly full → increases risk  

## Output

The system calculates an environmental risk level:

LOW  
MEDIUM  
HIGH  

## Future AI Extension

The rule-based system can be extended to:
- Machine learning models
- Environmental prediction systems
- Satellite data integration
- Launch event detection

This MVP demonstrates the concept of AI-assisted ecological monitoring.
