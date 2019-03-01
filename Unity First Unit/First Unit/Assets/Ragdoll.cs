using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Animator))]
public class Ragdoll : MonoBehaviour
{

    private Animator animator = null;
    public List<Rigidbody> rigibodies = new List<Rigidbody>();

    public bool RagdollOn
    {
        get { return !animator.enabled; }
        set
        {
            animator.enabled = !value;
            foreach (Rigidbody r in rigibodies)
                r.isKinematic = !value;
        }
    }

    // Use this for initialization
    void Start ()
    {
        animator = GetComponent<Animator>();
        Rigidbody[] rb = GetComponentsInChildren<Rigidbody>();

        foreach (Rigidbody r in rb)
        {
            rigibodies.Add(r);
            rigibodies[rigibodies.Count - 1].isKinematic = true;
        }
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
