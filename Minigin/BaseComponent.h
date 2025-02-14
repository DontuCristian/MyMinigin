#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------

//-----------------------------------------------------
// BComponent Class									
//-----------------------------------------------------
class BComponent final
{
public:
	BComponent() = default;				// Constructor
	~BComponent() = default;				// Destructor

	// -------------------------
	// Copy/move constructors and assignment operators
	// -------------------------    
	BComponent(const BComponent& other) = default;
	BComponent(BComponent&& other) noexcept = default;
	BComponent& operator=(const BComponent& other) = default;
	BComponent& operator=(BComponent&& other)	noexcept = default;

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	virtual void Update() { };
	virtual void FixedUpdate() { };

private:
	//-------------------------------------------------
	// Private member functions								
	//-------------------------------------------------


	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------


};

