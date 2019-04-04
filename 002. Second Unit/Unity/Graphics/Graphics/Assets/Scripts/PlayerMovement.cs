using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class PlayerMovement : MonoBehaviour
{

    public float m_playerSpeed;
    public Animator animator;
    public float m_playerRotate;
    

	// Use this for initialization
	void Start ()
    {
		
	}
	
	// Update is called once per frame
	void Update ()
    {
        Vector2 inputs = new Vector2(Input.GetAxis("Horizontal"), Input.GetAxis("Vertical"));

        transform.Translate(0, 0, inputs.y * m_playerSpeed * Time.deltaTime);
        transform.Rotate(Vector3.up, inputs.x * m_playerRotate * Time.deltaTime);

        //Vector3 moveMent = new Vector3(Input.GetAxis("Horizontal"), 0, Input.GetAxis("Vertical")) * m_playerSpeed * Time.deltaTime;

        //moveMent = this.gameObject.transform.rotation * moveMent;

        //this.gameObject.transform.position += moveMent;

        if (inputs.y < 0) {
            inputs.y *= -1f;
        }

        animator.SetFloat("Speed", inputs.y);

        //// the traform to face vector

        //if (moveMent.magnitude != 0)
        //{
        //  this.gameObject.transform.forward = moveMent.normalized;
        //}
	}

}
