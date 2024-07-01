import React from 'react'
import { KnobBase } from './knob_base'

describe('<KnobBase />', () => {
  it('renders', () => {
    // see: https://on.cypress.io/mounting-react
    cy.mount(<KnobBase />)
  })
})