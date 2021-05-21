typedef AnimalBase DZ_LightAI;

class AnimalBase extends Animal
{
	void AnimalBase()
	{	
		LockInventory(LOCK_FROM_SCRIPT); // Hides animals from vicinity in inventory. Remove this if wanted otherwise.
	}
}